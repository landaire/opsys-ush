#include <stdio.h>
#include "utility.h"
// add your #includes here

int main()
{
    char s[MAX];

    // You will need code to open .ushrc and .ush_history here

    printf("Please enter a string (exit to exit) ");
    fgets(s, MAX, stdin);
    strip(s);

    while(strcmp(s, "exit") != 0)
    {

        // You will need fork it and pipe it and other code here



        // you will probably need code to clean up stuff

        printf("Please enter a string (exit to exit) ");
        fgets(s, MAX, stdin);
        strip(s);

    }// end while

    return 0;

}// end main
