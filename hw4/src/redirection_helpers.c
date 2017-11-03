#include "sfish.h"

int redirection_index(char *user_args[], int argument_count)
{
    int redirect_index = false;
    for (int i = 0; i < argument_count; i++)
    {
        if (strcmp(user_args[i], ">") == 0 || strcmp(user_args[i], "<") == 0)
        {
            redirect_index = i;
            printf("String that was read: %s at index: %d\n", user_args[i], i);
        }
    }
    return redirect_index;
}

char* readjust_args(char *user_args[], int redirect_index)
{
    char* readjusted_args;

    for (int i = 0; i < redirect_index; i++)
    {
        readjusted_args = user_args[i];
        printf("%s\n", readjusted_args);
    }

    return readjusted_args;
}


