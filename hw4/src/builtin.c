#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>

void print_prompt()
{
    int prompt_len;
    char cwd_prompt[1024];

    char* tilda = "~";
    write(1, tilda, 1);
    getcwd(cwd_prompt, (1024 * sizeof(char)));

    prompt_len = strlen(cwd_prompt);
    write(1, cwd_prompt, prompt_len);
}

int num_args(char *source)
{
  // TODO: Do not hard code size
  char source_copy[1024];
  char *my_token;
  int num_args = 0;
  const char delim[2] = " ";

  // Now I have a copy of source which I can break
  strcpy(source_copy, source);

  // get the first token
  my_token = strtok(source_copy, delim);

  // walk through other tokens
  while (my_token != NULL)
  {
    num_args++;
    my_token = strtok(NULL, delim);
  }

  // TODO: ask TA about malloc and free for source_copy
  return num_args;
}

char** readline_parse(char *source, int size)
{
  // local variables
  int i = 0;
  char **argv = malloc(size * sizeof(char *));
  // TODO: Don't hardcode size
  char source_copy[1024];
  char *my_token;
  const char delim[2] = " ";

  strcpy(source_copy, source);

  my_token = strtok(source_copy, delim);

  while(my_token != NULL)
  {
    argv[i] = malloc(sizeof(char *));
    strcpy(argv[i], my_token);
    i++;
    my_token = strtok(NULL, delim);
  }

  // return the 2d array of the users arguments
  return argv;
}

void check_builtin(char *user_args[], int argument_count)
{
    if (strcmp(user_args[0], "help") == 0)
    {
        printf("%s\n", "Do you need help?!");
    }
    else if (strcmp(user_args[0], "pwd") == 0)
    {
        char *ptr;
        char buf [1024];
        ptr = getcwd(buf, sizeof(buf));

        printf("The filepath is: %s\n", ptr);
    }
    else if (strcmp(user_args[0], "cd") == 0 && argument_count == 1)
    {
        chdir(getenv("HOME"));
        return;
    }
    else if (strcmp(user_args[0], "cd") == 0 && argument_count > 1)
    {
        char *second_argument = user_args[1];
        int chdir_value = chdir(second_argument);

        if(chdir_value == -1)
        {
            perror("Some error");
        }
    }
    return;
}

bool check_exit(char *user_args[])
{
    bool exited = false;
    if (strcmp(user_args[0], "exit") == 0)
    {
        exited = true;
    }

    return exited;
}