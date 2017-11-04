#include "sfish.h"

void print_prompt()
{
  int prompt_len;
  char cwd_prompt[1024];

  char *home = getenv("HOME");
  getcwd(cwd_prompt, (MAXARG * sizeof(char*)));
  //printf("THIS IS CWD: %s THIS IS HOME: %s\n", cwd_prompt, home);

  if (strstr(cwd_prompt, home) != NULL)
  {
    char* tilda = "~";
    write(1, tilda, 1);
    strcpy(cwd_prompt, (cwd_prompt + strlen(home)));
  }
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
  char **argv = malloc(size * sizeof(char *) + 10);
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

  // execvp needs last element to be null
  argv[i] = '\0';
  // return the 2d array of the users arguments
  return argv;
}

void set_pwd()
{
  char *pwd;
  char buf [1024];
  pwd = getcwd(buf, sizeof(buf));
  setenv("PWD", pwd, 1);
}

bool check_builtin(char *user_args[], int argument_count)
{
  // TODO: declare this in a better spot
  int chdir_value;
  bool builtin_found = false;

  if (strcmp(user_args[0], "help") == 0)
  {
    if (argument_count > 2)
    {
      if ((strcmp(user_args[1], ">") == 0) && user_args[2] != NULL)
      {
        help_redirect = true;
        builtin_found = false;
        return builtin_found;
      }
    }
    else
    {
      printf("%s\n", "Do you need help?!");
      builtin_found = true;
    }
  }
  else if (strcmp(user_args[0], "pwd") == 0)
  {
    if (argument_count > 2)
    {
      if ((strcmp(user_args[1], ">") == 0) && user_args[2] != NULL)
      {
        pwd_redirect = true;
        builtin_found = false;
        return builtin_found;
      }
    }
    else
    {
      char *ptr;
      char buf [1024];
      ptr = getcwd(buf, sizeof(buf));
      builtin_found = true;

      printf("%s\n",ptr);
    }
  }
  else if (strcmp(user_args[0], "cd") == 0 && argument_count == 1)
  {
    builtin_found = true;
    setenv("OLDPWD", getenv("PWD"), 1);
    chdir(getenv("HOME"));
    set_pwd();

    // Not sure if I need this return here
    return builtin_found;
  }
  else if (strcmp(user_args[0], "cd") == 0 && argument_count > 1)
  {
    builtin_found = true;
    char *second_argument = user_args[1];
    if (strcmp(second_argument, "-") == 0)
    {
      chdir(getenv("OLDPWD"));
      setenv("OLDPWD", getenv("PWD"), 1);
      char *ptr;
      char buf [1024];
      ptr = getcwd(buf, sizeof(buf));

      printf("%s\n",ptr);
    }
    else
    {
      setenv("OLDPWD", getenv("PWD"), 1);
      chdir_value = chdir(second_argument);
    }

    if(chdir_value == -1)
    {
      perror("cd Error");
    }

    // TODO: cd - breaks for garbage value
    set_pwd();
  }
  return builtin_found;
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