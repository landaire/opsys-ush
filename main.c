#include <stdio.h>
#include <unistd.h>
#include "utility.h"
#include "linked_list.h"
#include "history.h"
#include "commands.h"
// add your #includes here

int main()
{
    char s[MAX];
    char **argv = NULL;
    int argc, hist_count = 100, hist_file_count = 1000;
    LinkedList *history = linkedList();

    // You will need code to open .ushrc and .ush_history here
    FILE *hist_file, *config_file;

    if (file_exists(HISTORY_FILENAME)) {
        hist_file = fopen(HISTORY_FILENAME, "r+");

        parse_history_file(hist_file, history);
    } else {
        hist_file = fopen(HISTORY_FILENAME, "w+");
        init_hist_start(0);
    }

    printf("Please enter a string (exit to exit) ");
    fgets(s, MAX, stdin);
    strip(s);


    while(strcmp(s, "exit") != 0)
    {
        // First thing, add this command to the history
        argv = makeargs(s, &argc);

        if (argc > 0) {
            add_to_history(history, (const char**)argv, argc, hist_count);


            printList(stdout, history, printCommand);
        }


        // you will probably need code to clean up stuff
        clean(argc, argv);
        argv = NULL;

        printf("Please enter a string (exit to exit) ");
        fgets(s, MAX, stdin);
        strip(s);
    }// end while

    // Truncate the history file so we only get commands we care about
    ftruncate(fileno(hist_file), 0);

    flush_history(hist_file, history, hist_count);

    fclose(hist_file);

    clearList(history, cleanCommand);
    free(history);
    history = NULL;

    return 0;

}// end main
