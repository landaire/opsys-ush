//
// Created by Lander Brandt on 4/23/15.
//

#ifndef HW7_BUILTIN_H
#define HW7_BUILTIN_H

typedef void (*executable_builtin)(int argc, char **argv);

typedef struct builtin_command {
    const char *command;
    executable_builtin exec;
} builtin;

void init_builtins();
void clean_builtins();
int command_is_builtin(const char *command);
void exec_builtin(int argc, char **command);
void builtin_history(int argc, char **command);
char *command_from_bang(char *command);
int is_bang_command(const char *command);

#endif //HW7_BUILTIN_H
