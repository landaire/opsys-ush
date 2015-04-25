//
// Created by Lander Brandt on 4/23/15.
//

#ifndef HW7_BUILTIN_H
#define HW7_BUILTIN_H

typedef int (*executable_builtin)(int argc, char **argv);

typedef struct builtin_command {
    const char *command;
    executable_builtin exec;
} builtin;

void init_builtins();
void clean_builtins();
int command_is_builtin(const char **command);
int exec_builtin(int argc, char **command);
int builtin_history(int argc, char **command);

#endif //HW7_BUILTIN_H
