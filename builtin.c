//
// Created by Lander Brandt on 4/23/15.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"
#include "ls.h"
#include "linked_list.h"
#include "history.h"
#include "commands.h"

static builtin **builtins = NULL;
static int builtin_count;

void init_builtins() {
    if (builtins != NULL) {
        return;
    }

    static const char *commands[] = {
            "ls",
            "history",
    };

    static executable_builtin exec[] = {
            ls,
            builtin_history,
    };

    int i;

    builtin_count = sizeof(commands);
    builtins = calloc(builtin_count, sizeof(builtin));

    builtin *temp;

    for (i = 0; i < builtin_count; i++) {
        temp = calloc(1, sizeof(builtin));

        temp->command = commands[i];
        temp->exec = exec[i];

        builtins[i] = temp;
    }
}

void clean_builtins() {
    int i;
    for (i = 0; i < builtin_count; i++) {
        free(builtins[i]);
    }

    free(builtins);

    builtin_count = 0;
    builtins = NULL;
}

int command_is_builtin(const char **command) {
    int i;

    for (i = 0; i < builtin_count; i++) {
        if (strcmp(command[0], builtins[i]->command) == 0) {
            return 1;
        }
    }

    return 0;
}

int exec_builtin(int argc, char **command) {
    int i;

    for (i = 0; i < builtin_count; i++) {
        if (strcmp(command[0], builtins[i]->command) == 0) {
            return builtins[i]->exec(argc, command);
        }
    }

    return -1;
}

int builtin_history(int argc, char **command) {
    printLastItems(stdout, history, printCommand, hist_count);

    return 0;
}
