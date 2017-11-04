#include "sfish.h"

void run_pipe(char *user_args[], int arg_count, char* input)
{

}

char** pipe_tok(char *source, int size)
{
    // local variables
    char **prog1 = malloc(size * sizeof(char *));
    //char **prog2 = malloc(size * sizeof(char *));

    int i = 0;
    char *pipe_token;
    const char delim[2] = "|";

    char source_copy[1024];
    strcpy(source_copy, source);

    // Now I can break source copy without worrying
    pipe_token = strtok(source_copy, delim);

    while(pipe_token != NULL)
    {
        prog1[i] = malloc(sizeof(char *));
        strcpy(prog1[i], pipe_token);
        i++;
        pipe_token = strtok(NULL, delim);
    }

    pipe_num = i;
    return prog1;
}

int num_pipes(char *user_args[], int arg_count)
{
    int pipe_count = 0;
    for (int i = 0; i < arg_count; i++)
    {
        if (strcmp(user_args[i], "|") == 0)
        {
            pipe_count++;
        }
    }
    return pipe_count;
}