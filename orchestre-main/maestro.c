#include "maestro.h"
#include "sockets/maestro_sockets.h"
#include "automation/parser.h"
#include "automation/commands.h"

int N_INSTRU = NB_MAX_INSTRU;

int main(int argc, char *argv[]) {
    uint16_t port = 1234; // Port défini par défaut
    int automode = 0;
    Command * commands = NULL;

    if (argc - 3 >
        0) { // Tester le nombre d'arguments, rappeler l'utilisation du progrm si trop d'arguments sont passés
        fprintf(stderr, "Usage %s [<\"scriptname\"> <automode>]\n", argv[0]);
        exit(1);
    }

    initOpenAL(); // initialisation de la bibliothèque de gestion du son
    initSocket(argv, port); // Initialisation du socket hote

    if (argc > 1) { // Lancer en mode automatique
        // parseFile(argv[1], &(commands));
    }

    awaitNewConnections(automode); //, commands); // Attendre les musiciens

    // une fois les zicos connected
    // runCommands(commands);

    free(commands);
    closeSocket();
    return 0;
}
