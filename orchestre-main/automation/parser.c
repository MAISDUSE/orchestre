#include "parser.h"
#include "commands.h"

#define MAX_LINE_SIZE 1024
#define MAX_CMD_SIZE 20

void removeNewline(char *line) {
    size_t ln = strlen(line) - 1;
    if (*line && line[ln] == '\n') {
        line[ln] = '\0';
    }
}

void getField(char *line, int num, char *fld) {
    char *tmp = strdup(line);
    removeNewline(tmp);
    const char *field = strtok(tmp, ";");
    int i = 0;

    while (field != NULL && i < num) {
        field = strtok(NULL, ";");
        i++;
    }
    if (field != NULL) {
        strncpy(fld, field, MAX_CMD_SIZE);
    } else {
        strncpy(fld, "\0", MAX_CMD_SIZE);
    }
    free(tmp);
}

void parseFile(char *name, Command * commands[]) {
    FILE *file;
    char line[MAX_LINE_SIZE];
    size_t line_count = 0;

    char cmd[MAX_CMD_SIZE];
    char params[MAX_CMD_SIZE];
    char musicos[MAX_CMD_SIZE];

    file = fopen(name, "r");

    if (file == NULL) {
        perror("Error opening automation script\n");
        exit(1);
    }

    // première lecture pour obtenir le nombre de lignes pour le malloc
    while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
        line_count++;
    }

    file = fopen(name, "r");
    int i;

    *commands = malloc(line_count * sizeof(Command));

    i = 0;
    int valid = 1;

    while (fgets(line, MAX_LINE_SIZE, file) != NULL && valid == 1) { // 2e lecture pour parser les commandes
        // Avec le getField, les ; vont être remplacés par des \0
        if (i != 0) {
            getField(line, 0, cmd);
            getField(line, 1, params);
            getField(line, 2, musicos);

            valid = checkCommand(&(*commands), i-1, cmd, params, musicos);
        }

        i++;
    }

    if (valid == 0) {
        free(*commands);
        fprintf(stderr, "Error parsing file -> line %d is not a valid command\n", i - 1);
        exit(1);
    }
}