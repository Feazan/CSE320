#include "sfish.h"

int out_redirect(char *user_args[], int arg_count)
{
  int right_pos = -1;
  for (int i = 0; i < arg_count; i++)
  {
    if (strcmp(user_args[i], ">") == 0)
    {
      // validate the redirect
      if (validate_args(user_args, arg_count, i) == true)
      {
        right_pos = i;
      }
      else
      {
        right_pos = -1;
      }
    }
  }

  return right_pos;
}

int in_redirect(char *user_args[], int arg_count)
{
  int left_pos = -1;
  for (int i = 0; i < arg_count; i++)
  {
    if (strcmp(user_args[i], "<") == 0)
    {
      // validate the redirect
      if (validate_args(user_args, arg_count, i) == true)
      {
        left_pos = i;
      }
      else
      {
        left_pos = -1;
      }
    }
  }

  return left_pos;
}

bool validate_args(char *user_args[], int arg_count, int redirect_pos)
{
  int num_redirect_copies = 0;
  char *redirect_copy = user_args[redirect_pos];
  bool valid_redirect = true;

  if (user_args[redirect_pos + 1] == NULL)
  {
    valid_redirect = false;
  }
  else if (strcmp(user_args[redirect_pos + 1], ">") == 0)
  {
    valid_redirect = false;
  }
  else if (strcmp(user_args[redirect_pos + 1], "<") == 0)
  {
    valid_redirect = false;
  }
  else
  {
    for (int i = 0; i < arg_count; i++)
    {
      if (strcmp(user_args[i], redirect_copy) == 0)
      {
        num_redirect_copies++;
      }
      if (num_redirect_copies > 1)
      {
        valid_redirect = false;
        break;
      }
    }
  }

  return valid_redirect;
}

void redirection(char *user_args[], int in_pos, int out_pos)
{
    if (in_pos != -1)
    {
        int fd0;
        fd0 = open(user_args[in_pos + 1], O_RDONLY, 0);
        if (fd0 < 0)
        {
          char buffer[50];
          sprintf(buffer, SYNTAX_ERROR, user_args[0]);
          write(1, buffer, strlen(buffer));
          _exit(1);
        }
        dup2(fd0, STDIN_FILENO);
        close(fd0);
    }

    if (out_pos != -1)
    {
        int fd1;
        fd1 = creat(user_args[out_pos + 1], 0666);
        if (fd1 < 0)
        {
          char buffer[50];
          sprintf(buffer, SYNTAX_ERROR, user_args[0]);
          write(1, buffer, strlen(buffer));
          _exit(1);
        }
        dup2(fd1, STDOUT_FILENO);
        close(fd1);
    }
}