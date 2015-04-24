//
// Created by Lander Brandt on 4/17/15.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sysexits.h>

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
