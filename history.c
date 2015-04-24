//
// Created by Lander Brandt on 4/23/15.
//

#include "history.h"
#include "commands.h"

void add_to_history(LinkedList *historyList, const char **command, int count) {
    if (can_add_to_history(historyList, command, count)) {
        addLast(historyList, buildNode(count, command, buildCommand));
    }
}

int can_add_to_history(const LinkedList *historyList, const char **command, int count) {
    if (historyList->size == 0) {
        return 1;
    }

    Commands *lastCommand = (Commands*)historyList->head->prev->data;
    return !commands_are_equal(lastCommand->rows, (const char**)lastCommand->command, count, command);
}
