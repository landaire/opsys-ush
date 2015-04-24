#ifndef PARSECOMMANDS_H
#define PARSECOMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

char ** parsePrePipe(char * s, int * count);
char ** parsePostPipe(char * s, int * count);

#endif // PARSECOMMANDS_H
