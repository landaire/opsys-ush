//
// Created by Lander Brandt on 4/23/15.
//

#ifndef HW7_HISTORY_H
#define HW7_HISTORY_H

#include "linked_list.h"
#include "commands.h"

#define HISTORY_FILENAME ".ush_history"

extern int history_start;
extern int hist_count;
extern int histfile_count;
extern LinkedList *history;

void add_to_history(char *command);
int can_add_to_history(char *command);
void flush_history(FILE *histfile);
void parse_history_file(FILE *histfile);
void init_hist_start(int start);
Commands *history_command_with_num(int num);

#endif //HW7_HISTORY_H
