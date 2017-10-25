#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "sfish.h"
#include "debug.h"

int main(int argc, char *argv[], char* envp[])
{
    char* input;
    bool exited = false;

    if(!isatty(STDIN_FILENO))
    {
        // If your shell is reading from a piped file
        // Don't have readline write anything to that file.
        // Such as the prompt or "user input"
        if((rl_outstream = fopen("/dev/null", "w")) == NULL){
            perror("Failed trying to open DEVNULL");
            exit(EXIT_FAILURE);
        }
    }

    do {
        input = readline(" :: fyaseen >> ");

        write(1, "\e[s", strlen("\e[s"));
        write(1, "\e[20;10H", strlen("\e[20;10H"));
        write(1, "SomeText", strlen("SomeText"));
        write(1, "\e[u", strlen("\e[u"));

        // If EOF is read (aka ^D) readline returns NULL
        if(input == NULL) {
            continue;
        }

        if(strcmp(input, "") != 0)
        {
            // At this point input stores the string user entered
            handle_help(input);
            handle_pwd(input);
            handle_cd(input);

            // Set the value of exit
            exited = handle_exit(input);
        }

        // In order to call the system exit(0) function
        if (exited == true)
        {
            // Readline mallocs the space for input. You must free it.
            rl_free(input);
            exit(0);
        }
        else
        {
            // Readline mallocs the space for input. You must free it.
            rl_free(input);
        }

    } while(!exited);

    debug("%s", "user entered 'exit'");

    return EXIT_SUCCESS;
}
