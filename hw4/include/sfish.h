#ifndef SFISH_H
#define SFISH_H

/* Format Strings */
#define EXEC_NOT_FOUND "sfish: %s: command not found\n"
#define JOBS_LIST_ITEM "[%d] %s\n"
#define STRFTIME_RPRMT "%a %b %e, %I:%M%p"
#define BUILTIN_ERROR  "sfish builtin error: %s\n"
#define SYNTAX_ERROR   "sfish syntax error: %s\n"
#define EXEC_ERROR     "sfish exec error: %s\n"

// Includes-----------------------------------------------
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <readline/readline.h>
#include "debug.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// Includes-----------------------------------------------

#define MAXARG 256

int pipe_num;
volatile sig_atomic_t pid;
bool help_redirect;
bool pwd_redirect;

// Builtin Helpers ---------------------------------------
int num_args(char *source);
char** readline_parse(char *source, int size);
bool check_builtin(char *user_args[], int argument_count);
bool check_exit(char *user_args[]);
void print_prompt();
void set_pwd();
// Builtin Helpers ---------------------------------------

// Executables Helpers -----------------------------------
typedef void handler_t(int);

void sigchld_handler(int s);
void sigint_handler(int s);
handler_t *Signal(int signum, handler_t *handler);
void unix_error(char *msg);

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
pid_t Fork(void);
int Sigsuspend(const sigset_t *set);
// Executables Helpers -----------------------------------

// Redirection Helpers -----------------------------------
int out_redirect(char *user_args[], int arg_count);
int in_redirect(char *user_args[], int arg_count);
bool validate_args(char *user_args[], int arg_count, int redirect_pos);
void redirection(char *user_args[], int in_pos, int out_pos);
// Redirection Helpers -----------------------------------

// Pipe Helpers -----------------------------------
void run_pipe(char *user_args[], int arg_count, char* input);
int num_pipes(char *user_args[], int arg_count);
char** pipe_tok(char *source, int size);
// Pipe Helpers -----------------------------------

#endif