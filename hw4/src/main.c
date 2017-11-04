#include "sfish.h"

int main(int argc, char *argv[], char* envp[])
{
    char* input;
    bool exited = false;
    bool builtin_found = false;


    sigset_t mask, prev;

    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGINT, sigint_handler);
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

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

 /*       write(1, "\e[s", strlen("\e[s"));
        write(1, "\e[20;10H", strlen("\e[20;10H"));
        write(1, "SomeText", strlen("SomeText"));
        write(1, "\e[u", strlen("\e[u"));*/

        // If EOF is read (aka ^D) readline returns NULL
        if(input == NULL) {
            continue;
        }

        if(strcmp(input, "") != 0)
        {
            pipe_num = 0;
            char **process = calloc(MAXARG, sizeof(char*));
            process = pipe_tok(input, MAXARG);
            //debug("p_num: %d", pipe_num);
            if (pipe_num > 1)
            {
                for (int i = 0; i < pipe_num; i++)
                {
                    int single_pc = 0;
                    char **p_vector = calloc(strlen(process[i] + 10), sizeof(char*));
                    // arg_count stores the number of user arguments
                    single_pc = num_args(process[i]);
                    printf("PROCESS[i]:%s\n", process[i]);
                    // arg_vector stores an array of the users input
                    p_vector = readline_parse(process[i], single_pc);
                    //debug("single_pc: %d", single_pc);
                    run_pipe(p_vector, single_pc, process[i]);
                }
            }
            else
            {
                // arg_count stores the number of user arguments
                int arg_count = num_args(input);
                // arg_vector stores an array of the users input
                char **arg_vector = readline_parse(input, arg_count);

                builtin_found = check_builtin(arg_vector, arg_count);
                exited = check_exit(arg_vector);

                if (!builtin_found)
                {
                    Sigprocmask(SIG_BLOCK, &mask, &prev);
                    if ((Fork()) == 0)
                    {
                        Sigprocmask(SIG_SETMASK, &prev, NULL);
                    // REDIRECTION OCCURS HERE BEFORE EXECVP
                        int in = in_redirect(arg_vector, arg_count);
                        int out = out_redirect(arg_vector, arg_count);

                        if (in != -1 || out != -1)
                        {
                            if (in != -1 && out == -1)
                            {
                                arg_vector[in] = NULL;
                            }
                            if (in == -1 && out != -1)
                            {
                                arg_vector[out] = NULL;
                            }
                            redirection(arg_vector, in, out);
                        }

                        if (execvp(arg_vector[0], arg_vector) < 0)
                        {
                            printf("%s\n", "Command Not Found");
                            _exit(0);
                        }
                    }

                    pid = 0;
                    while (!pid)
                    {
                        Sigsuspend(&prev);
                    }

                    Sigprocmask(SIG_SETMASK, &prev, NULL);
                }
            }
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