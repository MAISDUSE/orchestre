#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <dirent.h>
#include <errno.h>

#if defined(__APPLE__)
#include "freealut/alut.h"
#else
#include <AL/alut.h>
#endif


#ifndef MAESTRO
#define MAESTRO

typedef struct {
    int sockfd;
    long int nthr;
    int automode;
    ALuint buffer;
    ALuint source;
    int playing;
    int loadedSource;
} Musician;

#define NB_MAX_INSTRU 2

// Actions
#define PLAY_PAUSE 1
#define CHG_POSITION 2
#define CHG_DIRECTION 3
#define CHG_INSTRUMENT 4
#define QUIT 0

Musician musicians[NB_MAX_INSTRU];

#endif