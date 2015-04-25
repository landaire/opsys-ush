//
// Created by Lander Brandt on 4/7/15.
//

#include "ls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <langinfo.h>
#include <unistd.h>
#include <libgen.h>

int ls(int argc, char ** argv) {
    DIR *dp = NULL;
    struct dirent *dirp;
    struct stat fs;
    int lswitchPresent = 0;
    char *path = NULL;
    int is_file = 0;


    if(argc < 2 || argc > 3) {
        perror("Not enough arguments\n");
        exit(-1);
    }// end if

    // Iterate our args and parse them
    int i;
    for (i = 1; i < argc; i++) {
        // Assume the only arg without a dash is going to be the path
        if (argv[i][0] != '-') {
            path = argv[i];
        } else if (!strcmp(argv[i], "-l")) {
            lswitchPresent = 1;
        }
    }


    if((dp = opendir(path)) == NULL) {
        if (access(path, F_OK) == -1) {
            perror("Could not open file or directory\n");
            exit(-1);
        } else {
            is_file = 1;
        }
    }

    if (is_file) {
        char *name = basename(path);
        if (!lswitchPresent) {
            printDirentNormal(name);
        } else {
            lstat(path, &fs);
            printLongFormat(name, &fs);
        }
    } else {
        while((dirp = readdir(dp)) != NULL) {
            if (!lswitchPresent) {
                printDirentNormal(dirp->d_name);
            } else {
                lstat(dirp->d_name, &fs);
                printLongFormat(dirp->d_name, &fs);
            }
        }

        closedir(dp);
    }

    exit(0);
}// end main

void printDirentNormal(char *name) {
    printf("%s\n", name);
}

void printLongFormat(char *name, struct stat *fs) {
    struct group    *g;
    struct passwd   *pwd;
    struct tm       *mod_time;
    char date_string[MAX];


    // TODO: probably leaks
    g = getgrgid(fs->st_gid);
    pwd = getpwuid(fs->st_uid);

    // type
    printDirentType(fs);
    printPerms(fs);
    printf(" %4d ", fs->st_nlink);

    // print the owner and group
    // TODO: clean up probable leak here
    printf(" %s %s ", pwd->pw_name, g->gr_name);

    // print file size
    printf(" %10d ", (int)fs->st_size);

    // print the date
    #ifdef __APPLE__
    mod_time = localtime(&fs->st_mtimespec.tv_sec);
    #else
    mod_time = localtime(&fs->st_mtime);
    #endif
    strftime(date_string, sizeof(date_string), nl_langinfo(D_T_FMT), mod_time);
    printf(" %s ", date_string);

    printf("%s\n", name);
}

/**
 * Kind of like the linux sperm()
 */
void printPerms(struct stat *fs) {
    static int perms[] = {
            // user flags
            S_IRUSR, S_IWUSR, S_IXUSR,
            // group flags
            S_IRGRP, S_IWGRP, S_IXGRP,
            // owner flags
            S_IROTH, S_IWOTH, S_IXOTH,
    };

    int i;

    for (i = 0; i < PERMS_COUNT; i++) {
        char flag = '-';
        if (fs->st_mode & perms[i]) {
            // read flag
            if (i % 3 == 0) {
                flag = 'r';
            // executable flag
            } else if (i % 2 == 0) {
                flag = 'x';
            } else {
                flag = 'w';
            }
        }

        putchar(flag);
    }
}

void printDirentType(struct stat *fs) {
    char type = '?';

    if (S_ISREG(fs->st_mode)) {
        type = '-';
    } else if (S_ISLNK(fs->st_mode)) {
        type = 'l';
    } else if (S_ISDIR(fs->st_mode)) {
        type = 'd';
    } else if (S_ISBLK(fs->st_mode)) {
        type = 'b';
    }  else if (S_ISCHR(fs->st_mode)) {
        type = 'c';
    }  else if (S_ISLNK(fs->st_mode)) {
        type = 'l';
    }  else if (S_ISSOCK(fs->st_mode)) {
        type = 's';
    }

    putchar(type);
}
