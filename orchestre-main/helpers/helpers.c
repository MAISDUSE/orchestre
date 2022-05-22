#include "helpers.h"

int isInt(char * supposed_int) {
    char validation[CHAR_SIZE];
    int entier = atoi(supposed_int);

    sprintf(validation, "%d", entier);

    return strncmp(supposed_int, validation, CHAR_SIZE) == 0;
}

int getInt(char * msg) {
    char response[CHAR_SIZE];
    printf("%s", msg);
    scanf("%s", response);

    int c;
    while((c = getchar()) != '\n' && c != EOF); // Vider le buffer

    if (isInt(response)) {
        return atoi(response);
    } else {
        printf("Erreur : veuillez indiquer un nombre valide\n");
        return getInt(msg);
    }
}
/*
float getFloat(char * msg) {
    char response[CHAR_SIZE];
    char validation[CHAR_SIZE];
    float reel;
    printf("%s", msg);
    scanf("%s", response);

    int c;
    while((c = getchar()) != '\n' && c != EOF); // Vider le buffer

    reel = (float) atof(response);

    sprintf(validation, "%f", reel);

    printf("%s : %s\n", response, validation);

    if (strncmp(response, validation, CHAR_SIZE) == 0) {
        return reel;
    } else {
        printf("Erreur : veuillez indiquer un nombre valide\n");
        return getFloat(msg);
    }
}*/

void sendAck(int sockfd) {
    int ack = 1;
    if (send(sockfd, &ack, sizeof(ack), 0) < 0) {
        perror("Error sending ack\n");
        exit(1);
    }
}
