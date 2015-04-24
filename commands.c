//
// Created by Lander Brandt on 4/10/15.
//

#include "commands.h"

void printCommand(void * passedIn) {
    Commands *command = (Commands*)passedIn;

    int i;
    printf("%5d", command->num);
    for (i = 0; i < command->rows; i++) {
        printf(" %s", command->command[i]);
    }

    printf("\n");
}

void * buildCommand(int argc, char ** argv) {
    static int num = 0;
    Commands *command = calloc(1, sizeof(Commands));
    command->command = calloc((size_t)(argc + 1), sizeof(char*));
    command->num = num++;
    command->rows = argc;

    size_t i;
    for (i = 0; i < argc; i++) {
        command->command[i] = calloc(strlen(argv[i]) + 1, sizeof(char));
        strcpy(command->command[i], argv[i]);
    }

    return command;
}

int compareTwoCommands(const void * p1, const void * p2) {
    Commands **com1 = (Commands**)p1;
    Commands **com2 = (Commands**)p2;

    return (*com1)->num - (*com2)->num;
}

void cleanCommand(void * command) {
    Commands *com = (Commands*)command;
    int i;
    for (i = 0; i < com->rows; i++) {
        free(com->command[i]);
    }

    free(com->command);
    free(com);
}
