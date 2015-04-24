//
// Created by Lander Brandt on 4/23/15.
//

#ifndef HW7_BUILTIN_H
#define HW7_BUILTIN_H

int command_is_builtin(const char **command) {
    static const char *builtin[] = {
            "history",
    };
}

int exec_builtin(const char **command, int argc) {

}

#endif //HW7_BUILTIN_H
