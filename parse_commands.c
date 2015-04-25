//
// Created by Lander Brandt on 4/17/15.
//

#include "utility.h"
#include "linked_list.h"
#include "commands.h"
#include "parse_commands.h"

LinkedList *parse_command(char *s) {
    // shitty names but temp_argv is for handling whether or not there's redirection
    // since makeargs will return a NULL pointer if there's no tokens
    char *temp_argv[2], **redirectionParts, **pipeParts, **argv;
    int redirectionCount, pipeCount, argc, i;
    LinkedList *returnList = linkedList();

    // First parse by redirection
    redirectionParts = makeargs(s, &redirectionCount, ">");

    if (redirectionCount != -1) {
        temp_argv[0] = redirectionParts[1];
        temp_argv[1] = NULL;

        addLast(returnList, build_node(build_command_part(1, temp_argv, COMMAND_TYPE_REDIRECT)));
    } else {
        temp_argv[0] = s;
        redirectionParts = temp_argv;
    }

    pipeParts = makeargs(redirectionParts[0], &pipeCount, "|");

    // There were commands piped, so iterate through them, break up the args, add to list
    if (pipeCount != -1) {
        argv = makeargs(s, &argc, " ");
        addFirst(returnList, build_node(build_command_part(1, argv, COMMAND_TYPE_EXECUTE)));
    } else {
        // This loop is decrementing because if there was a pipe redirection, we always want it to be
        // last so we call addFirst
        for (i = pipeCount - 1; i >= 0; i--) {
            argv = makeargs(pipeParts[i], &argc, " ");
            addFirst(returnList, build_node(build_command_part(1, argv, COMMAND_TYPE_EXECUTE)));
        }
    }

    // Redirection parts is the weird case where if there's no redirection, we don't touch it.
    if (redirectionCount != -1) {
        clean(redirectionCount, redirectionParts);
    }

    if (pipeCount != -1) {
        clean(pipeCount, pipeParts);
    }

    return returnList;
}
