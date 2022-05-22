#include "musician_sockets.h"

int client_sockfd;
struct sockaddr_in client_addr;

void initSocket(uint16_t port) {
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (client_sockfd < 0) {
        perror("Error creating socket\n");
        exit(1);
    }

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(client_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr) < 0) {
        perror("Error creating socket adress\n");
        exit(1);
    }
}

void connectSocket() {
    // la connexion déclenche la création du thread distant
    assert(connect(client_sockfd, (struct sockaddr *) &client_addr, sizeof(client_addr)) >= 0);

    long int automode;

    printf("Connexion avec le maestro...\n");

    if (recv(client_sockfd, &automode, sizeof(automode), 0) <= 0) {
        printf("Le maestro est injoignable ou ses postes de musiciens sont complets\n");
        exit(1);
    }

    if (automode == 0) {
        printf("Connecté, bienvenue.\n");
        int m = -1;
        do
        {
            m = menu();
            handle_menu(client_sockfd, m);
        } while (m != 0);
    } else {
        printf("Le maestro est en mode automatique, détendez vous et profitez du spectacle.\n");
    }

    close(client_sockfd);
}