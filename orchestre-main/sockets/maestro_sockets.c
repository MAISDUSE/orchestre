#include "maestro_sockets.h"

#define MSG_SIZE 1024
#define MAX_FILE_COUNT 30

extern int N_INSTRU;

int host_sockfd;
struct sockaddr_in host_addr;
int host_addrlen;
int newSocket;
long int nbThreads = 0;

Musician createMusician(int sockfd, long int nthr, long int automode) {
    Musician musicos;

    musicos.sockfd = sockfd;
    musicos.nthr = nthr;
    musicos.automode = (int) automode;
    musicos.buffer = 0;
    musicos.source = 0;
    musicos.playing = 0;
    musicos.loadedSource = 0;

    return musicos;
}

void receiveVector(Musician * musicos, float * vec, size_t size) {
    if (recv(musicos->sockfd, vec, size, 0) <= 0) {
        perror("Error reciving vector\n");
        exit(1);
    }
}

void getInstrumentName(Musician * musicos, char * name) {
    int ack;
    int file_index = 0;
    char filenames[MAX_FILE_COUNT][MSG_SIZE];

    DIR * d = opendir("./sounds");
    struct dirent *dir;

    if (d) {
        while (recv(musicos->sockfd, &ack, sizeof(ack), 0) != 0 && (dir = readdir(d)) != NULL) {
            strncpy(filenames[file_index], dir->d_name, MSG_SIZE);
            // printf("Filename id[%d] : %s\n", file_index, filenames[file_index]);
            if (send(musicos->sockfd, filenames[file_index], sizeof(filenames[file_index]), 0) < 0) {
                perror("Error sending filename to musician\n");
                exit(1);
            }
            file_index++;
        }

        closedir(d);
        dir = NULL;
    } else {
        if (recv(musicos->sockfd, &ack, sizeof(ack), 0) < 0) {
            perror("Error receiving ack\n");
        }
        fprintf(stderr, "Not a file or folder !\n");
    }

    // Arreter la réception des noms de fichiers côté client
    strncpy(filenames[file_index + 1], "*", MSG_SIZE); // * => indique la fin de l'envoi
    if (send(musicos->sockfd, filenames[file_index + 1], sizeof(filenames[file_index + 1]), 0) < 0) {
        perror("Error sending end tag\n");
        exit(1);
    }

    // Lire le fichier sélectioné
    assert(recv(musicos->sockfd, &file_index, sizeof(file_index), 0) > 0);

    // Chemin du fichier sélectionné
    strncpy(name, "sounds/", MSG_SIZE);
    strncat(name, filenames[file_index], MSG_SIZE);
}

void handleMusicianActions(Musician * musicos) {
    int choice;
    float vec[3];
    char name[MSG_SIZE];
    int state;
    char response[MSG_SIZE];

    while (recv(musicos->sockfd, &choice, sizeof(choice), 0) != 0) {

        switch (choice) {

            case PLAY_PAUSE: // Play Pause
                state = playPause(musicos);
                getFeedbackString(response, PLAY_PAUSE, state, MSG_SIZE);
                break;

            case CHG_POSITION: // Changement de position
                receiveVector(musicos, vec, sizeof(vec));
                state = changePosition(musicos, vec);
                getFeedbackString(response, CHG_POSITION, state, MSG_SIZE);
                break;

            case CHG_DIRECTION: // Changement d'orientation
                receiveVector(musicos, vec, sizeof(vec));
                state = changeOrientation(musicos, vec);
                getFeedbackString(response, CHG_DIRECTION, state, MSG_SIZE);
                break;

            case CHG_INSTRUMENT: // Changement d'instrument
                getInstrumentName(musicos, name);
                changeSource(musicos, name);
                strncpy(response, "Instrument modifié", MSG_SIZE);
                break;

            case QUIT:
                strncpy(response, "Au revoir", MSG_SIZE);
                break;

            default:
                strncpy(response, "Command unknown", MSG_SIZE);
                break;
        }
        if (send(musicos->sockfd, response, sizeof(response), 0) <= 0) {
            perror("Error sending feedback message\n");
            exit(1);
        }
    }
}

// Initialise le socket maestro
void initSocket(char *argv[], uint16_t port) {
    int init = 0;
    initSem(1, argv[0], &init);

    host_sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket

    if (host_sockfd < 0) {
        perror("Error creating socket\n");
        exit(1);
    }

    int opt = 1;
    // Pas possible de combiner les 2 filtres en un avec un OU binaire pour execution sur mac
    int optsock1 = setsockopt(host_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int optsock2 = setsockopt(host_sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    if (optsock1 != 0 || optsock2 != 0) {
        perror("Error setting socket options\n");
        exit(1);
    }

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = INADDR_ANY;
    host_addrlen = sizeof(host_addr);

    int binding = bind(host_sockfd, (struct sockaddr *) &host_addr, sizeof(host_addr));

    if (binding < 0) {
        perror("Error binding socket\n");
        exit(1);
    }

    int listenning = listen(host_sockfd, 3);

    if (listenning != 0) {
        perror("Error listenning to socket\n");
        exit(1);
    }

    printf("Maestro socket init complete\n");
}

void closeSocket() {
    close(host_sockfd);
}

// Thread maestro gestion des sockets musiciens
void *thread_maestro(void *args) {

    int sockfd = ((int *) args)[0];
    long int nthr = ((long int *) args)[1];
    long int automode = ((long int *) args)[2];

    musicians[nthr] = createMusician(sockfd, nthr, automode);

    V(0);

    if (send(sockfd, &automode, sizeof(automode), 0) <= 0) {
        perror("Error connecting to musician\n");
        exit(1);
    }
    printf("Musicien connecté : %d\n", sockfd);

    if (automode == 0) { // Gérer les actions entre sockets

        handleMusicianActions(&(musicians[nthr]));

    } else { // Mode automatique
        printf("Mode automatique.\n");
    }

    close(sockfd);
    deleteSourceBuffer(&(musicians[nthr]));
    nbThreads--;

    printf("Musicien déconnecté : %d\n", sockfd);

    return NULL;
}

// Thread maestro connexion des sockets musiciens
void awaitNewConnections(int automode) {
    pthread_t thr[N_INSTRU];
    void *args[3];

    printf("Waiting for musicians\n");

    while (nbThreads < N_INSTRU) // attente des N_INSTRU connexion
    {
        int new_socket;
        // attente passive de nouveaux clients
        new_socket = accept(host_sockfd, (struct sockaddr *) &host_addr, (socklen_t * ) & host_addrlen);

        if (new_socket < 0) {
            perror("Error accepting new socket\n");
            exit(1);
        }

        args[0] = (void *) (long int) new_socket;
        args[1] = (void *) nbThreads;
        args[2] = (void *) (long int) automode;

        int th_info = pthread_create(&thr[nbThreads], NULL, &thread_maestro, args);
        nbThreads++;

        if (th_info != 0) {
            perror("Error creating client thread\n");
            exit(1);
        }

        P(0);
    }

    libereSem();

    for (long int i = 0; i < N_INSTRU - 1; ++i) {
        if (pthread_join(thr[i], NULL) != 0) {
            perror("Error joining threads\n");
            exit(1);
        }
    }
}
