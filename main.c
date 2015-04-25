#include <stdio.h>
#include "utility.h"
#include "linked_list.h"
#include "history.h"
#include "commands.h"
// add your #includes here

int main()
{
    char s[MAX];
    char **argv = NULL;
    int argc;
    LinkedList *history = linkedList();

    // You will need code to open .ushrc and .ush_history here
    FILE *histFile, *configFile;

    if (file_exists(HISTORY_FILENAME)) {
        histFile = fopen(HISTORY_FILENAME, "rw");


    }

    printf("Please enter a string (exit to exit) ");
    fgets(s, MAX, stdin);
    strip(s);


    while(strcmp(s, "exit") != 0)
    {
        // First thing, add this command to the history
        argv = makeargs(s, &argc);

        if (argc > 0) {
            add_to_history(history, (const char**)argv, argc);


            printList(stdout, history, printCommand);
        }


        // you will probably need code to clean up stuff
        clean(argc, argv);
        argv = NULL;

        printf("Please enter a string (exit to exit) ");
        fgets(s, MAX, stdin);
        strip(s);
    }// end while

    clearList(history, cleanCommand);
    free(history);
    history = NULL;

    return 0;

}// end main
