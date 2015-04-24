//
// Created by Lander Brandt on 4/10/15.
//

#include "commands.h"

void printCommand(FILE *outFile, void * passedIn) {
    Commands *command = (Commands*)passedIn;

    int i;
    fprintf(outFile, "%5d", command->num);
    for (i = 0; i < command->rows; i++) {
        fprintf(outFile, " %s", command->command[i]);
    }

    fprintf(outFile, "\n");
}

void * buildCommand(int argc, const char ** argv) {
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

int commands_are_equal(int com_a_count, const char **com_a, int com_b_count, const char **com_b) {
    if (com_a_count != com_b_count) {
        return 0;
    }

    int i;
    for (i = 0; i < com_a_count; i++) {
        if (strcmp(com_a[i], com_b[i]) != 0) {
            return 0;
        }
    }

    return 1;
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
