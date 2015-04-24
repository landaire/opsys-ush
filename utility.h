#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

void strip(char *s);
void clean(int argc, char **argv);
void printargs(int argc, char **argv);
char ** makeargs(char *s, int * argc);

#endif
