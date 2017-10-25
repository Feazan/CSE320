#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>

bool check_for_builtin(char *source, char *word_to_check)
{
    // Local variables
    char *first_argumment;
    const char s[2] = " ";
    int cmp_ret;

    // Copy the string becasue token will break  it
    char *source_copy = malloc(sizeof(source));
    strcpy(source_copy, source);

    // This should contain the first argument
    first_argumment = strtok(source, s);

    cmp_ret = strcmp(first_argumment, word_to_check);

    // Copy back
    strcpy(source, source_copy);
    free(source_copy);

    // Now check if the first argument equals word to check
    if(cmp_ret == 0)
        return true;
    else
        return false;
}

void display_help()
{
    write(1, "Do you need help?", strlen("Do you need help?"));
    write(1, "\n", strlen("\n"));
}

void handle_help(char *source)
{
    if (check_for_builtin(source, "help") == true)
    {
        display_help();
    }
}

bool handle_exit(char *source)
{
    bool exited = false;
    if (check_for_builtin(source, "exit") == true)
    {
        exited = true;
    }

    return exited;
}

void handle_pwd(char *source)
{
    char *ptr;

    if (check_for_builtin(source, "pwd") == true)
    {
        char buf [1024];
        ptr = getcwd(buf, sizeof(buf));

        printf("%s\n", ptr);
    }
}

int num_arguments(char *source)
{
    int num_args = 0;
    char *source_copy = malloc(sizeof(source));
    char *my_token;

    // The copy has the original string
    strcpy(source_copy, source);

    const char s[2] = " ";

    /* get the first token */
    my_token = strtok(source_copy, s);

    /* walk through other tokens */
    while (my_token != NULL)
    {
        num_args++;
        my_token = strtok(NULL, s);
    }

    return num_args;
}

char* get_second_argument(char *source)
{
    int second_arg = 0;
    char *source_copy = malloc(sizeof(source));
    char *my_token;
    char *second_argument;

    // The copy has the original string
    strcpy(source_copy, source);

    const char s[2] = " ";

    /* get the first token */
    my_token = strtok(source_copy, s);

    while (second_arg < 2)
    {
        second_argument = my_token;
        my_token = strtok(NULL, s);
        second_arg++;
    }

    return second_argument;
}

void handle_cd(char *source)
{
    char *second_argument;

    if (num_arguments(source) == 1)
    {
        if (check_for_builtin(source, "cd") == true)
        {
            chdir(getenv("HOME"));
            return;
        }
    }
    else if (num_arguments(source) > 1)
    {
        // get the second argument
        second_argument = get_second_argument(source);
        int chdir_value = chdir(second_argument);

        if(chdir_value == -1)
        {
            perror("Some error");
        }
    }
    return;
}