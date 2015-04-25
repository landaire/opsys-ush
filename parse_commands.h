#ifndef PARSECOMMANDS_H
#define PARSECOMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "linked_list.h"

#define COMMAND_TYPE_EXECUTE 1
#define COMMAND_TYPE_REDIRECT 2

LinkedList *parse_command(char *s);

#endif // PARSECOMMANDS_H
