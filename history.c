//
// Created by Lander Brandt on 4/23/15.
//

#include "history.h"
#include "commands.h"
#include "utility.h"
#include "parse_commands.h"
#include "pipe_commands.h"

int history_start = -1;
int hist_count = 100;
int histfile_count = 1000;
LinkedList *history = NULL;

void add_to_history(char *command) {
    Commands *last_command;
    Node *first_element, *temp;
    int id;

    if (can_add_to_history(command)) {
        // check the size to ensure we only keep HISTCOUNT items
        // TODO: don't think this is needed? doing this may lead to bugs
        if (history->size == hist_count) {
//            // remove the first element
//            first_element = history->head->next;
//
//            temp = first_element->next;
//
//            cleanCommand(first_element->data);
//            free(first_element);
//
//            history->head->next = temp;
//            temp->prev = history->head;
        }

        if (command[0] == '!' && sscanf(command, "!%d", &id) == 1) {
            last_command = history_command_with_num(id);
            command = last_command->command;
        }

        addLast(history, build_node(buildCommand(command)));
    }
}

int can_add_to_history(char *command) {
    Commands *current_command = NULL;
    int id;

    if (history->size == 0) {
        return 1;
    }

    if (strcmp(command, "!!") == 0) {
        return 0;
    }

    if (command[0] == '!' && sscanf(command, "!%d", &id) == 1) {
        current_command = history_command_with_num(id);
        // invalid identifier...
        if (current_command == NULL) {
            return 0;
        }

        command = current_command->command;
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

Commands *history_command_with_num(int num) {
    Node *cur;
    Commands *cur_command;

    cur = history->head;

    while ((cur = cur->next) != history->head) {
        cur_command = (Commands*)cur->data;
        if (cur_command->num == num) {
            return cur_command;
        }
    }

    return NULL;
}
