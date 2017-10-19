#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>


long size;
char *buf;
char *ptr;
char *token;
char *current_directory;



void modify_prompt()
{
     // Turn this into a function
    size = pathconf(".", _PC_PATH_MAX);


    if ((buf = (char *)malloc((size_t)size)) != NULL)
    {
        ptr = getcwd(buf, (size_t)size);
    }

    const char s[2] = "/";

        /* get the first token */
    token = strtok(ptr, s);

        /* walk through other tokens */
    while (token != NULL)
    {
            //printf("%s\n", token);
        current_directory = token;
        token = strtok(NULL, s);
    }
        // Turn this into a function

    write(1, "~/", strlen("~/"));
    write(1, current_directory, strlen(current_directory));
}