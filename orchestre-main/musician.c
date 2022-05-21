#include "musician.h"

int menu()
{
	int response;

	printf("\nMenu du musicien : \n");
	printf("[1] : Play/Pause\n");
	printf("[2] : Position de la source\n");
	printf("[3] : Orientation de la source\n");
	printf("[4] : Changer d'instrument\n");
	printf("[0] : Quitter\n");
    printf("Votre choix : ");

    scanf("%d", &response);

    int c;
    while((c = getchar()) != '\n' && c != EOF); // Vider le buffer en cas de reception d'autre chose que des entiers

	/* if (scanf("%d", &response) != 1) {
        printf("Veuillez indiquer un chiffre valide\n");

        return menu(); // Redemander la saisie

    } else {
        return response;
    }*/

    return response;
}

void handle_menu(int sockfd, int menu)
{
	int unknown = 0;
	float vec[3];
	int inpt;
	char msg[1024];
	int ack = 1;
	char filenames[1024];
	int file_index;

	assert(send(sockfd, &menu, sizeof(menu), 0) > 0);

	switch (menu)
	{
		case 1:
			break;

		case 2:
			printf("\nMise à jour des coordonnées :\n");
			printf("X : ");
			scanf("%f", &(vec[0]));

            int c;
            while((c = getchar()) != '\n' && c != EOF); // Vider le buffer

			printf("Y : ");
            scanf("%f", &(vec[1]));

            while((c = getchar()) != '\n' && c != EOF); // Vider le buffer

			printf("Z : ");
            scanf("%f", &(vec[2]));

            while((c = getchar()) != '\n' && c != EOF); // Vider le buffer

			assert(send(sockfd, vec, sizeof(vec), 0) > 0);
			break;

		case 3:
			printf("\nMise à jour de la direction -> la source regarde le point :\n");
			printf("X : ");
			scanf("%d", &inpt);
			vec[0] = inpt;

			printf("Y : ");
			scanf("%d", &inpt);
			vec[1] = inpt;

			printf("Z : ");
			scanf("%d", &inpt);
			vec[2] = inpt;

			assert(send(sockfd, vec, sizeof(vec), 0) > 0);
			break;

		case 4:
			file_index = 0;
			assert(send(sockfd, &ack, sizeof(ack), 0) > 0);
			printf("\nInstruments disponibles :\n");
			
			while(recv(sockfd, filenames, sizeof(filenames), 0) != 0 && strcmp(filenames, "*") != 0)
			{
				if (file_index > 1) { // On retire l'affichage pour les 2 premiers fichiers -> ce sont . et ..
					printf("[%d] %s\n", file_index - 1, filenames);
				}

				file_index++;
				assert(send(sockfd, &ack, sizeof(ack), 0) > 0);
			}

			printf("Sélectionner l'instrument à jouer : ");
			scanf("%d", &file_index);
			file_index++;
			assert(send(sockfd, &file_index, sizeof(file_index), 0) > 0);

			break;
		case 0:
			break;

		default:
			unknown = 1;
			break;
	}

	if (unknown == 0)
	{

	}
	else
	{
		unknown = 0;
		printf("Option inconnue\n");
	}

    assert(recv(sockfd, msg, sizeof(msg), 0) > 0);
    printf("--> %s\n", msg);
}

int main(int argc, char *argv[])
{
	if (argc - 1 != 1)
	{
		fprintf(stderr, "Usage %s PORT\n", argv[0]);
		exit(10);
	}

	int v = atoi(argv[1]);
	assert(v > 0 && v < 65535);
	uint16_t port = (uint16_t)v;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
	assert(sockfd >= 0);

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	assert(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) >= 0);
	// la connexion déclenche la création du thread distant
	assert(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) >= 0);
    long int automode;

    printf("Connexion avec le maestro...\n");

    if (recv(sockfd, &automode, sizeof(automode), 0) <= 0) {
        printf("Le maestro est injoignable ou ses postes de musiciens sont complets\n");
        exit(1);
    }

    if (automode == 0) {
        printf("Connecté, bienvenue.\n");
        int m = -1;
        do
        {
            m = menu();
            handle_menu(sockfd, m);
        } while (m != 0);
    } else {
        printf("Le maestro est en mode automatique, détendez vous et profitez du spectacle.\n");
    }

	/* int duration;
	assert(recv(sockfd, &duration, sizeof(duration), 0) > 0);

	printf("Duration of song : %d\n", duration); */

	printf("done!\n");
	close(sockfd);
	return 0;
}
