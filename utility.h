#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

char *strip(char *s);
void clean(int argc, char **argv);
void printargs(int argc, char **argv);
char ** makeargs(char *s, int * argc, const char *delimiter);
int file_exists(const char *filename);

#endif
