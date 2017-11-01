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
bool check_builtin(char *user_args[], int argument_count);
bool check_exit(char *user_args[]);
void print_prompt();
void set_pwd();
// Builtin Helpers ---------------------------------------

// Executables Helpers -----------------------------------
extern volatile sig_atomic_t pid;
typedef void handler_t(int);

void sigchld_handler(int s);
void sigint_handler(int s);
handler_t *Signal(int signum, handler_t *handler);
void unix_error(char *msg);

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
pid_t Fork(void);
int Sigsuspend(const sigset_t *set);
// Executables Helpers -----------------------------------