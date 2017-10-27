#ifndef SFISH_H
#define SFISH_H

/* Format Strings */
#define EXEC_NOT_FOUND "sfish: %s: command not found\n"
#define JOBS_LIST_ITEM "[%d] %s\n"
#define STRFTIME_RPRMT "%a %b %e, %I:%M%p"

#endif

// Builtin Helpers ---------------------------------------
int num_args(char *source);
char** readline_parse(char *source, int size);
void check_builtin(char *user_args[], int argument_count);
bool check_exit(char *user_args[]);
void print_prompt();
void set_pwd();
// Builtin Helpers ---------------------------------------



// Executables Helpers -----------------------------------
void executables(char *user_args[], int argument_count);
// Executables Helpers -----------------------------------