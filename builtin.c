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
#include "pipe_commands.h"

static builtin **builtins = NULL;
static size_t builtin_count = 2;

int is_bang_command(const char *command) {
    return command[0] == '!';
}

void init_builtins() {
    if (builtins != NULL) {
        return;
    }

    static const char *commands[] = {
            "myls",
            "history",
    };

    static executable_builtin exec[] = {
            ls,
            builtin_history,
    };

    int i;

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

int command_is_builtin(const char *command) {
    int i;

    if (is_bang_command(command)) {
        return 1;
    }

    for (i = 0; i < builtin_count; i++) {
        if (strcmp(command, builtins[i]->command) == 0) {
            return 1;
        }
    }

    return 0;
}

void exec_builtin(int argc, char **command) {
    int i;
    char *bang_command;

    if (is_bang_command(command[0])) {
        bang_command = command_from_bang(command[0]);

        if (bang_command != NULL) {
            execute_command(bang_command);
        }

        free(bang_command);

        exit(EXIT_SUCCESS);
    }

    for (i = 0; i < builtin_count; i++) {
        if (strcmp(command[0], builtins[i]->command) == 0) {
            // It's assumed that the command should exit themselves... if they don't, it's a failure
            builtins[i]->exec(argc, command);
        }
    }

    exit(EXIT_FAILURE);
}

char *command_from_bang(char *command) {
    Commands *cur_command;
    int id, argc = 0;

    char *out = NULL;
    char **argv = NULL;

    // Check if it's a double bang
    if (strcmp(command, "!!") == 0) {
        cur_command = (Commands*)history->head->prev->data;

        argv = makeargs(cur_command->command, &argc, " ");
    } else if (sscanf(command, "!%d", &id) == 1) {
        cur_command = history_command_with_num(id);

        if (cur_command != NULL) {
            argv = makeargs(cur_command->command, &argc, " ");
        }
    }

    if (argv != NULL) {
        if (argc > 0) {
            out = calloc(strlen(argv[0]) + 1, sizeof(char));
            strcpy(out, argv[0]);
        }

        clean(argc, argv);
    }

    return out;
}

void builtin_history(int argc, char **command) {
    printLastItems(stdout, history, printCommand, hist_count);

    exit(EXIT_SUCCESS);
}
