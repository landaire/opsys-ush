//
// Created by Lander Brandt on 4/23/15.
//

#ifndef HW7_HISTORY_H
#define HW7_HISTORY_H

#include "linked_list.h"

#define HISTORY_FILENAME ".ush_history"

extern int history_start;
extern int hist_count;
extern int histfile_count;
extern LinkedList *history;

void add_to_history(const char **command, int count);
int can_add_to_history(const char **command, int count);
void flush_history(FILE *histfile, int hist_count);
void parse_history_file(FILE *histfile);
void init_hist_start(int start);

#endif //HW7_HISTORY_H
