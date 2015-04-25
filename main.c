#include <stdio.h>
#include <unistd.h>
#include "utility.h"
#include "linked_list.h"
#include "history.h"
#include "commands.h"
#include "builtin.h"
#include "pipe_commands.h"
// add your #includes here

#define CONFIG_FILENAME ".ushrc"
#define HISTCOUNT_KEY "HISTCOUNT"
#define HISTFILECOUNT_KEY "HISTFILECOUNT"

int main()
{
    /**
     * Begin init
     */
    char s[MAX];
    char **argv = NULL, *command;
    int argc;
    history = linkedList();

    // You will need code to open .ushrc and .ush_history here
    FILE *hist_file, *config_file;

    // Parse the user config file
    if (file_exists(CONFIG_FILENAME)) {
        config_file = fopen(CONFIG_FILENAME, "r");

        while (fscanf(config_file, "%s=%d", s, &argc)) {
            if (strcmp(HISTCOUNT_KEY, s) == 0) {
                hist_count = argc;
            } else if (strcmp(HISTFILECOUNT_KEY, s)) {
                histfile_count = argc;
            }
        }

        fclose(config_file);
    }


    if (file_exists(HISTORY_FILENAME)) {
        hist_file = fopen(HISTORY_FILENAME, "r+");

        parse_history_file(hist_file);
    } else {
        hist_file = fopen(HISTORY_FILENAME, "w+");
        init_hist_start(0);
    }

    init_builtins();

    /**
     * End init
     *
     * Begin ush loop
     */

    printf("Please enter a string (exit to exit) ");
    fgets(s, MAX, stdin);
    strip(s);

    while(strcmp(s, "exit") != 0)
    {
        // strip leading/trailing whitespace from the command
        command = strip(s);

        if (strlen(command) > 0) {
            add_to_history(command);
            execute_command(command);
        }

        printf("%s\n", command);

        printf("Please enter a string (exit to exit) ");
        fgets(s, MAX, stdin);
        strip(s);
    }// end while

    /**
     * Begin history flushing
     */

    // Truncate the history file so we only get commands we care about
    ftruncate(fileno(hist_file), 0);
    rewind(hist_file);

    flush_history(hist_file);

    fclose(hist_file);

    /**
     * End history flushing
     *
     * Begin cleanup
     */

    clearList(history, cleanCommand);
    free(history);
    history = NULL;

    clean_builtins();

    return 0;

}// end main
