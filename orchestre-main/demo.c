// gcc -O3 -Wall -Wextra -Wconversion -ansi -Wpedantic -Werror -std=c11 -pthread -lopenal -lalut demo.c semaphore.c -o demo
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>

#if defined(__APPLE__)
#include "freealut/alut.h"
#else
#include <AL/alut.h>
#endif


#include "semaphore.h"

#define N_INSTRU 10

ALuint sources[N_INSTRU];

void* thread_client(void *args)
{
	/*
	 * Les infos passées au thread sont minimes
	 * Il devrait être plus intéressant d'avoir
	 * un moyen de donner une configuration:
	 * position de la source, orientation de la
	 * source, fichier à lire, etc.
	 */
	int sockfd = ((int*)args)[0];
	long int nthr = ((long int*)args)[1];

	#define MAX_LEN 50
	char name[MAX_LEN];
	switch(nthr)
	{
		case 0:
			strncpy(name, "sounds/uplifting-flute.wav", MAX_LEN);
			break;
		case 1:
			strncpy(name, "sounds/tribal-flute.wav", MAX_LEN);
			break;
		case 2:
			strncpy(name, "sounds/whistle-toy.wav", MAX_LEN);
			break;
		default:
			strncpy(name, "sounds/sample-12s.wav", MAX_LEN);
			break;
	}
	V(0);
	printf("%s connected to socket %d...\n", name, sockfd);

	ALuint b = alutCreateBufferFromFile(name);
	ALuint s = sources[nthr];

    // lien buffer -> source
    alGenSources(1, &s);
    alSourcei(s, AL_BUFFER, (ALint)b);
	/*
	 * Ici on fait lire le son dès le départ
	 * ce serait mieux de se bloquer sur une
	 * lecture de la socket pour savoir quand
	 * démarrer la lecture, stopper la lecture
	 * éventuellement changer la position ou
	 * l'orientation, etc.
	 */
	alSourcePlay(s);

	char c;
	ssize_t n;
	while((n=read(sockfd, &c, 1))!=0); // une boucle infinie...

	alDeleteSources(1, &s);
	alDeleteBuffers(1, &b);
	close(sockfd);
	return NULL;
}

int main(int argc, char* argv[])
{
	if (argc-1!=1)
	{
		fprintf(stderr, "Usage %s PORT\n", argv[0]);
		exit(10);
	}

	int v = atoi(argv[1]);
	assert(v>0 && v<65535);
	uint16_t port = (uint16_t)v;

	int init=0;
    initSem(1,argv[0],&init);

    // initialisation de la bibliothèque de gestion du son
    alutInit(0, NULL);
    alGetError();

    // position par défaut pour l'écoute
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
	assert(sockfd>=0);

    int opt = 1;
    int optsock1 = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	int optsock2 = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
	assert(optsock1==0 && optsock2 == 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int addrlen = sizeof(addr); 

    assert(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))>=0);
    assert(listen(sockfd, 3)==0);

	pthread_t thr[N_INSTRU];
	long int nthr = 0;
	void* args[2];

	// attente des N_INSTRU connexion
    while(nthr<N_INSTRU)
    {
    	int new_socket;
    	// attente passive de nouveaux clients
        assert((new_socket = accept(sockfd, (struct sockaddr *)&addr, (socklen_t*)&addrlen))>=0);

		/*
		 * on ne passe que quelques arguments :
		 * numéro de la socket d'écoute
		 * indice du thread (qui sert à savoir à
		 * quelle source se connecter)
		 */
		args[0] = (void*)(long int)new_socket;
		args[1] = (void*)nthr;
		
        assert(pthread_create(&thr[nthr], NULL, &thread_client, args)==0);
        P(0);
        nthr++;
    }

	libereSem();
	for (long int i=0; i<nthr; ++i)
	{
		assert(pthread_join(thr[i], NULL)==0);
	}

	return 0;
}
