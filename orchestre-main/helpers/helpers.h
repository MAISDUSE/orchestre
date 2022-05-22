#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#ifndef HELPERS
#define HELPERS

#define CHAR_SIZE 1024

int getInt(char * msg);
// float getFloat(char * msg);
void sendAck(int sockfd);

#endif