#include "../maestro.h"
#include "../audio/maestro_audio.h"
#include "../menu/maestro_menu.h"
#include "semaphore.h"

#ifndef MAESTRO_SOCKETS
#define MAESTRO_SOCKETS

void initSocket(char * argv[], uint16_t port);
void awaitNewConnections(int automode);

#endif