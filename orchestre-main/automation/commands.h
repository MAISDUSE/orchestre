#include "../maestro.h"

#ifndef COMMANDS
#define COMMANDS

int checkCommand(Command * commands[], int index, char * cmd, char * params, char * musicos);
void runCommands(Command commands[]);

#endif