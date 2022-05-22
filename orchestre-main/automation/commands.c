#include "commands.h"
#include "../helpers/helpers.h"

int checkCommand(Command * commands[], int index, char * cmd, char * params, char * musicos) { // TODO: completer les vérifications
    int valid = 1;

    if (strcmp(cmd, "create") == 0) {
        if (isInt(params)) {
            commands[index]->cmd = CREATE;
            commands[index]->param1 = atoi(params);
        } else {
            valid = 0;
        }
    } else if (strcmp(cmd, "play") == 0) {
        if (isInt(params) && isInt(musicos)) {
            int playPause = atoi(params);
            int zicos = atoi(musicos);

            if ((playPause == 0 || playPause == 1) && commands[0]->param1 > zicos) {
                commands[index]->cmd = PLAY;
                commands[index]->param1 = playPause;
                commands[index]->musician = zicos;
            } else {
                valid = 0;
            }
        } else {
            valid = 0;
        }
    } else if (strcmp(cmd, "position") == 0) {

    } else if (strcmp(cmd, "direction") == 0) {

    } else if (strcmp(cmd, "music") == 0) {
        if (isInt(musicos) && commands[0]->param1 > atoi(musicos)) {
            if (access(params, R_OK) == 0) {
                commands[index]->cmd = MUSIC;
                strncpy(commands[index]->music, params, 1024);
                commands[index]->musician = atoi(musicos);
            } else {
                fprintf(stderr, "The file %s does not exist\n", params);
                valid = 0;
            }
        } else {
            valid = 0;
        }

    } else if (strcmp(cmd, "end") == 0){

    } else if (strcmp(cmd, "wait") == 0) {
        if (isInt(params)) {
            commands[index]->cmd = WAIT;
            commands[index]->param1 = atoi(params);
        } else {
            valid = 0;
        }
    } else {
        valid = 0;
    }

    return valid;
}

void runCommands(Command commands[]) {
    (void) commands;
}