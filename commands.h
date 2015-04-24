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

void printCommand(void * passedIn);
void * buildCommand(int argc, char ** argv);
int compareTwoCommands(const void * p1, const void * p2);
void cleanCommand(void *);


#endif // COMMANDS_H
