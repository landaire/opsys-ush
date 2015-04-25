//
// Created by Lander Brandt on 4/17/15.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sysexits.h>
#include "linked_list.h"
#include "parse_commands.h"
#include "commands.h"
#include "builtin.h"

void execute_command(char *command) {
    int *pipes, read_fd = -1, status, stdinfd, stdoutfd, i, child_num;
    pid_t *child_pids, child_pid;
    LinkedList *commands;
    FILE *redirect = NULL;
    Node *cur;
    command_part *current_command, *next_command = NULL;

    commands = parse_command(command);

    pipes = calloc(commands->size * 2, sizeof(int));
    child_pids = calloc(commands->size * 2, sizeof(pid_t));

    for (i = 0; i < commands->size; i++) {
        if (pipe(pipes + (i * 2)) < 0) {
            perror("Could not pipe");
            exit(EXIT_FAILURE);
        }
    }

    cur = commands->head;

    child_num = 0;
    /**
     * Should probably not care about the linkedlist implementation... but screw it
     */
    while ((cur = cur->next) != commands->head) {
        current_command = (command_part*)cur->data;

        // skip redirect ones
        if (current_command->type == COMMAND_TYPE_REDIRECT) {
            continue;
        }

        if (cur->next != commands->head) {
            next_command = (command_part*)cur->next->data;

            if (next_command->type == COMMAND_TYPE_REDIRECT) {
                // create the file to redirect to
                redirect = fopen(next_command->command[0], "w+");
                dup2(fileno(redirect), pipes[child_num * 2 + 1]);
            }
        }

        printf("forking\n");
        child_pid = fork();
        child_pids[child_num] = child_pid;

        if (child_pid == -1) {
            perror("fork error");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Redirect stdin only if it's not the first command
            if (child_num > 0) {
//                printf("in child %d -- redirecting stdin to %d\n", child_num, (child_num - 1) * 2);
                // get the read pipe for the last command
                if (dup2(pipes[(child_num - 1) * 2], STDIN_FILENO) < 0) {
                    perror("dup2 error at stdin");
                    exit(EXIT_FAILURE);
                }
            }

            // only redirect stdout if this isn't the last command
            if (cur->next != commands->head) {
//                printf("in child %d -- redirecting stdout to %d\n", child_num, child_num * 2 + 1);
                // write to the next child's read fd
                if (dup2(pipes[(child_num * 2) + 1], STDOUT_FILENO) < 0) {
                    perror("dup2 error at stdout");
                    exit(EXIT_FAILURE);
                }
            }

//            printf("executing %s\n", current_command->command[0]);

            for (i = 0; i < commands->size * 2; i++) {
                close(pipes[i]);
            }

            // Check if the command is builtin
            if (command_is_builtin(current_command->command[0])) {
                exec_builtin(current_command->count, current_command->command);
            } else {
                execvp(current_command->command[0], current_command->command);
            }

            // couldn't execute the process for whatever reason...
            exit(EXIT_FAILURE);
        }

        if (redirect != NULL) {
            fclose(redirect);
            redirect = NULL;
        }

        next_command = NULL;
        child_num++;
    }

    // Close pipes
    for (i = 0; i < commands->size * 2; i++) {
        close(pipes[i]);
    }

    // Wait for child processes and report their status
    for (i = 0, cur = commands->head->next; i < commands->size; i++, cur = cur->next) {
        current_command = (command_part*)cur->data;

        waitpid(child_pids[i], &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) {
            fprintf(stderr, "Invalid command %s\n", current_command->command[0]);
        }
    }

    // Cleanup
    fflush(stdout);

    clearList(commands, clean_command_part);
    free(commands);
    free(pipes);
    free(child_pids);
}

void pipeIt(char ** prePipe, char ** postPipe) {
    int fd[2], res;
    pid_t prePipePid, postPipePid;

    res = pipe(fd);

    if(res < 0)
    {
        printf("Pipe Failure\n");
        return;
    }// end if

    if((prePipePid = fork()) == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);

        int status = execvp(prePipe[0], prePipe);

        _exit(status);
    }

    if ((postPipePid = fork()) == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);

        int status = execvp(postPipe[0], postPipe);

        _exit(status);
    }

    int status;

    close(fd[1]);

    pid_t pids[2] = {prePipePid, postPipePid};

    int i;
    for (i = 0; i < sizeof(pids); i++) {
        pid_t returnPid = waitpid(pids[i], &status, 0);
        if (WEXITSTATUS(status) > EX__BASE || returnPid == -1) {
            printf("Invalid command: %s\n", prePipe[0]);
        }
    }
}
