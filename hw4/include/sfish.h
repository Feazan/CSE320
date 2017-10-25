#ifndef SFISH_H
#define SFISH_H

/* Format Strings */
#define EXEC_NOT_FOUND "sfish: %s: command not found\n"
#define JOBS_LIST_ITEM "[%d] %s\n"
#define STRFTIME_RPRMT "%a %b %e, %I:%M%p"

#endif

void handle_help(char *source);
bool check_for_builtin(char *source, char *word_to_check);
void display_help();
bool handle_exit(char *source);
void handle_pwd(char *source);
void handle_cd(char *source);

int num_arguments(char *source);
char* get_second_argument(char *source);