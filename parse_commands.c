//
// Created by Lander Brandt on 4/17/15.
//

#include "utility.h"

char ** parsePrePipe(char * s, int * count) {
    int totalCount = 0;
    char **args = makeargs(s, &totalCount);

    int pipeFound = 0, i;

    (*count) = 0;

    for (i = 0; i < totalCount; i++) {
        if (!pipeFound && !strcmp(args[i], "|")) {
            pipeFound = 1;
        }

        if (pipeFound) {
            free(args[i]);
            args[i] = NULL;
        } else {
            (*count)++;
        }
    }

    return args;
}

char ** parsePostPipe(char * s, int * count) {
    int totalCount = 0;
    char **args = makeargs(s, &totalCount);

    int pipeFound = 0, i, j;

    (*count) = 0;

    for (i = totalCount - 1; i >= 0; i--) {
        if (!pipeFound && !strcmp(args[i], "|")) {
            pipeFound = 1;
        }

        if (pipeFound) {
            free(args[i]);
            args[i] = NULL;
        } else {
            (*count)++;
        }
    }

    char **returnArgs = calloc((size_t)(totalCount + 1), sizeof(char*));

    int startingIndexForRightSide = (totalCount - *count);

    for (i = startingIndexForRightSide, j = 0; i < totalCount; i++, j++) {
        returnArgs[j] = args[i];
    }

    free(args);

    return returnArgs;
}
