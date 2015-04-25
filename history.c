//
// Created by Lander Brandt on 4/23/15.
//

#include "history.h"
#include "commands.h"
#include "utility.h"

int history_start = -1;
int hist_count = 100;
int hist_file_count = 1000;
LinkedList *history = NULL;

void add_to_history(const char **command, int count) {
    if (can_add_to_history(history_list, command, count)) {
        // check the size to ensure we only keep HISTCOUNT items
        if (history_list->size == hist_count) {
            // remove the first element
            Node *firstElement = history_list->head->next;
            Node *temp = firstElement->next;
            cleanCommand(firstElement->data);
            free(firstElement);

            history_list->head->next = temp;
            temp->prev = history_list->head;
        }

        addLast(history_list, buildNode(count, command, buildCommand));
    }
}

int can_add_to_history(const char **command, int count) {
    if (history->size == 0) {
        return 1;
    }

    Commands *lastCommand = (Commands*)history->head->prev->data;
    return !commands_are_equal(lastCommand->rows, (const char**)lastCommand->command, count, command);
}

void parse_history_file(FILE *histfile) {
    int index, argc;
    char command[MAX], **argv;

    while (fscanf(histfile, "%5d %s\n", &index, command) == 2) {

        // Only set history_start once. This is because the first line in the file is going to be
        // the lowest index, and all indices are sequential. In buildCommand we do history_start + num++,
        // so they'll match.
        if (history_start == -1) {
            history_start = index;
        }


        argv = makeargs(command, &argc);

        addLast(history, buildNode(argc, (const char**)argv, buildCommand));

        free(argv);
    }

    // Init history start to 0 if the history file was empty
    init_hist_start(0);
}

void flush_history(FILE *histfile, int hist_count) {
    printLastItems(histfile, history, printCommand, hist_count);
}

void init_hist_start(int start) {
    if (history_start != -1) {
        return;
    }

    history_start = start;
}
