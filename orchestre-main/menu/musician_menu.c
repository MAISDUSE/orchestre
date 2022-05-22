#include "musician_menu.h"
#include "../helpers/helpers.h"

int menu() {
    printf("\nMenu du musicien : \n");
    printf("[1] : Play/Pause\n");
    printf("[2] : Position de la source\n");
    printf("[3] : Orientation de la source\n");
    printf("[4] : Changer d'instrument\n");
    printf("[0] : Quitter\n");

    return getInt("Votre choix : ");
}

void handle_menu(int sockfd, int menu) {
    float vec[3];
    char msg[CHAR_SIZE];
    char filenames[CHAR_SIZE];
    int file_index;

    if (send(sockfd, &menu, sizeof(menu), 0) < 0) {
        perror("Error sending menu choice\n");
        exit(1);
    }

    switch (menu) {
        case 2:
            printf("\nMise à jour des coordonnées :\n");
            vec[0] = (float) getInt("X : ");
            vec[1] = (float) getInt("Y : ");
            vec[2] = (float) getInt("Z : ");

            if (send(sockfd, vec, sizeof(vec), 0) < 0) {
                perror("Error sending coordinates\n");
                exit(1);
            }
            break;

        case 3:
            printf("\nMise à jour de la direction -> la source regarde le point :\n");
            vec[0] = (float) getInt("X : ");
            vec[1] = (float) getInt("Y : ");
            vec[2] = (float) getInt("Z : ");


            if (send(sockfd, vec, sizeof(vec), 0) < 0) {
                perror("Error sending coordinates\n");
                exit(1);
            }
            
            break;

        case 4:
            file_index = 0;
            sendAck(sockfd);
            printf("\nInstruments disponibles :\n");

            while (recv(sockfd, filenames, sizeof(filenames), 0) != 0 &&
                   strncmp(filenames, "*", CHAR_SIZE) != 0) { // * -> arrêt de lecture des fichiers
                if (file_index > 1) { // On retire l'affichage pour les 2 premiers fichiers -> ce sont . et ..
                    printf("[%d] %s\n", file_index - 1, filenames);
                }

                file_index++;
                sendAck(sockfd);
            }

            file_index = getInt("Sélectionner l'instrument à jouer : ");
            file_index++; // Réindexer pour matcher avec le serveur, on a fait -1 pour l'affichage

            if (send(sockfd, &file_index, sizeof(file_index), 0) < 0) {
                perror("Error sending file index\n");
                exit(1);
            }
            break;

        default:
            break;
    }

    if (recv(sockfd, msg, sizeof(msg), 0) < 0) {
        perror("Error receiving feedback\n");
        exit(1);
    }

    printf("--> %s\n", msg);
}