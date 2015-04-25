#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct commands
{
    char ** command;
    int rows;
    int num;
};

typedef struct commands Commands;

void printCommand(FILE *out_file, void * passedIn);
void * buildCommand(int argc, const char ** argv);
int commands_are_equal(int com_a_count, const char **com_a, int com_b_count, const char **com_b);
int compareTwoCommands(const void * p1, const void * p2);
void cleanCommand(void *);


#endif // COMMANDS_H
