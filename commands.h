#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct commands
{
    char *command;
    int num;
};

typedef struct command_part {
    char **command;
    int count;
    // COMMAND_TYPE_x
    int type;

} command_part;

typedef struct commands Commands;

void printCommand(FILE *out_file, void * passedIn);
void * buildCommand(char *s);
int commands_are_equal(char *com_a, char *com_b);
int compareTwoCommands(const void * p1, const void * p2);
void cleanCommand(void *);

void clean_command_part(void *);
void *build_command_part(int argc, char **argv, int type);


#endif // COMMANDS_H
