#include "maestro.h"
#include "sockets/maestro_sockets.h"

int N_INSTRU = NB_MAX_INSTRU;

int main(int argc, char* argv[])
{
    uint16_t port = 1234; // Port défini par défaut
    int automode = 0;

    if (argc - 2 > 0) { // Tester le nombre d'arguments, rappeler l'utilisation du progrm si trop d'arguments sont passés
        fprintf(stderr, "Usage %s [scriptname]\n", argv[0]);
        exit(1);
    }

    if (argc - 1 == 1) { // Lancer en mode automatique
        // TODO: Test si un script est bien passé ou si "--help" ou "-h" est , tester si les commandes sont correctes
        printf("2e param : %s\n", argv[1]);

    }

    initOpenAL(); // initialisation de la bibliothèque de gestion du son
    initSocket(argv, port); // Initialisation du socket hote

    awaitNewConnections(automode); // Attendre les musiciens

	return 0;
}
