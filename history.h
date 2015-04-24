//
// Created by Lander Brandt on 4/23/15.
//

#ifndef HW7_HISTORY_H
#define HW7_HISTORY_H

#include "linked_list.h"

#define HISTORY_FILENAME ".ush_history"

void add_to_history(LinkedList *historyList, const char **command, int count);
int can_add_to_history(const LinkedList *historyList, const char **command, int count);
void flush_history_to_file(FILE *histfile);
void parse_(FILE *histfile);

#endif //HW7_HISTORY_H
