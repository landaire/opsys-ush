//
// Created by Lander Brandt on 4/10/15.
//

#include "commands.h"
#include "history.h"

extern int history_start;

void printCommand(FILE *out_file, void * passedIn) {
    Commands *command = (Commands*)passedIn;

    fprintf(out_file, "%5d %s\n", command->num, command->command);
}

void * buildCommand(char *s) {
    static int num = 0;
    Commands *command = calloc(1, sizeof(Commands));

    command->command = calloc(strlen(s) + 1, sizeof(char));
    strcpy(command->command, s);

    command->num = history_start + num++;

    return command;
}

int compareTwoCommands(const void * p1, const void * p2) {
    Commands **com1 = (Commands**)p1;
    Commands **com2 = (Commands**)p2;

    return (*com1)->num - (*com2)->num;
}

int commands_are_equal(char *com_a, char *com_b) {
    return strcmp(com_a, com_b) == 0;
}

void cleanCommand(void * command) {
    Commands *com = (Commands*)command;
    free(com->command);
    free(com);
}

void clean_command_part(void *data) {
    command_part *part = (command_part*)data;
    int i;
    for (i = 0; i < part->count; i++) {
        free(part->command[i]);
    }

    free(part->command);
    free(part);
}

void *build_command_part(int argc, char **argv, int type) {
    static int num = 0;
    command_part *command = calloc(1, sizeof(command_part));
    command->command = calloc((size_t)(argc + 1), sizeof(char*));
    command->count = argc;
    command->type = type;

    size_t i;
    for (i = 0; i < argc; i++) {
        command->command[i] = calloc(strlen(argv[i]) + 1, sizeof(char));
        strcpy(command->command[i], argv[i]);
    }

    return command;
}
