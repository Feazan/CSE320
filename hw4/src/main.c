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
    int arg_count;
    char **arg_vector;
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
        print_prompt();
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
            // arg_count stores the number of user arguments
            arg_count = num_args(input);
            // arg_vector stores an array of the users input
            arg_vector = readline_parse(input, arg_count);

            check_builtin(arg_vector, arg_count);
            exited = check_exit(arg_vector);

            executables(arg_vector, arg_count);
        }

        // Currently nothing is implemented
        printf(EXEC_NOT_FOUND, input);

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
            // TODO: free arg_vector and all its elements
            rl_free(input);
        }

    } while(!exited);

    debug("%s", "user entered 'exit'");

    return EXIT_SUCCESS;
}
