#include <sys/stat.h>
#include "utility.h"

char *strip(char *s)
{
    int x = 0;
    size_t len;
    len = strlen(s);
    char *end;

    // Trim trailing linefeeds and such
    while(s[x] != '\0' && x < len)
    {
        if(s[x] == '\r')
            s[x] = '\0';

        else if(s[x] == '\n')
            s[x] = '\0';

        x++;
    }// end while

    // Trim leading spaces
    while(*s == ' ') {
        s++;
    }

    end = s + strlen(s) - 1;

    // Trim trailing spaces
    while (end > s && *end == ' ') {
        end--;
    }

    *(end + 1) = '\0';

    return s;
}// end strip

void clean(int argc, char **argv)
{
    int i;
    for (i = 0; i < argc; i++) {
        free(argv[i]);
    }

    free(argv);
}

void printargs(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++)
		printf("%s\n", argv[x]);

}

char ** makeargs(char *s, int * argc, const char *delimiter)
{
    size_t index = 0;
    char *tokens[MAX];
    char *save = calloc(strlen(s) + 1, sizeof(char)), *token = NULL, *orig = save;

    strcpy(orig, s);

    token = strtok_r(orig, delimiter, &save);

    if (token == NULL) {
        *argc = -1;
        return NULL;
    }

    do {
        char *tok = calloc(strlen(token) + 1, sizeof(char));
        strcpy(tok, token);
        tokens[index++] = tok;
    } while ((token = strtok_r(NULL, delimiter, &save)) != NULL);

    // + 1 for the null index
    char **returnTokens = calloc(index + 1, sizeof(char*));
    // Copy all pointers to the returnTokens

    int i;
    for (i = 0; i < index; i++) {
        returnTokens[i] = tokens[i];
    }

   *argc = (int)index;

    free (orig);

   return returnTokens;

}// end makeArgs

int file_exists(const char *filename) {
    struct stat filestat;
    return stat(filename, &filestat) == 0;
}




