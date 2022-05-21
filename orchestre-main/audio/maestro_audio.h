#include "../maestro.h"

#ifndef MAESTRO_AUDIO
#define MAESTRO_AUDIO

void initOpenAL();
int playPause(Musician * musicos);
int changePosition(Musician * musicos, float vec[]);
int changeOrientation(Musician * musicos, float vec[]);
void changeSource(Musician * musicos, char * name);

#endif