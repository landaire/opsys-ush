//
// Created by Lander Brandt on 4/23/15.
//

#include "history.h"
#include "commands.h"
#include "utility.h"
#include "parse_commands.h"

int history_start = -1;
int hist_count = 100;
int histfile_count = 1000;
LinkedList *history = NULL;

void add_to_history(char *command) {
    if (can_add_to_history(command)) {
        // check the size to ensure we only keep HISTCOUNT items
        if (history->size == hist_count) {
            // remove the first element
            Node *firstElement = history->head->next;
            Node *temp = firstElement->next;
            cleanCommand(firstElement->data);
            free(firstElement);

            history->head->next = temp;
            temp->prev = history->head;
        }

        addLast(history, build_node(buildCommand(command)));
    }
}

int can_add_to_history(char *command) {
    if (history->size == 0) {
        return 1;
    }

    Commands *lastCommand = (Commands*)history->head->prev->data;
    return !commands_are_equal(lastCommand->command, command);
}

void parse_history_file(FILE *histfile) {
    int index;
    char command[MAX], line[MAX];

    while (fgets(line, MAX, histfile) != NULL) {
        if (sscanf(line, "%5d %s", &index, command) != 2) {
            continue;
        }
        // Only set history_start once. This is because the first line in the file is going to be
        // the lowest index, and all indices are sequential. In buildCommand we do history_start + num++,
        // so they'll match.
        if (history_start == -1) {
            history_start = index;
        }

        addLast(history, build_node(buildCommand(command)));
    }

    // Init history start to 0 if the history file was empty
    init_hist_start(0);
}

void flush_history(FILE *histfile) {
    printLastItems(histfile, history, printCommand, histfile_count);
}

void init_hist_start(int start) {
    if (history_start != -1) {
        return;
    }

    history_start = start;
}
