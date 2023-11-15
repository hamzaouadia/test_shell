#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define read_buf_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

#define USEft_getline 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinf - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@line_count_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinf
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int line_count_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf;
	int cmd_buf_type;
	int readfd;
	int histcount;
} inf_t;

#define inf_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(inf_t *);
} builtin_table;


int ft_hsh(inf_t *, char **);
int ft_find_builtin(inf_t *);
void ft_find_cmd(inf_t *);
void ft_fork_cmd(inf_t *);

int ft_is_cmd(inf_t *, char *);
char *ft_dup_chars(char *, int, int);
char *ft_find_path(inf_t *, char *, char *);

int loopft_hsh(char **);

void ft_eputs(char *);
int ft_eputchar(char);
int ft_putfd(char c, int fd);
int ft_putsfd(char *str, int fd);

int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

char **strtow(char *, char *);
char **strtow2(char *, char);

char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

int ft_bfree(void **);

int ft_interactive(inf_t *);
int ft_is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

int ft_erratoi(char *);
void print_error(inf_t *, char *);
int print_d(int, int);
char *ft_convert_number(long int, int, int);
void ft_remove_comments(char *);

int ft_myexit(inf_t *);
int ft_mycd(inf_t *);
int ft_myhelp(inf_t *);

int ft_myhistory(inf_t *);
int ft_myalias(inf_t *);

ssize_t ft_get_input(inf_t *);
int ft_getline(inf_t *, char **, size_t *);
void ft_sigHandler(int);

void ft_clear_inf(inf_t *);
void ft_set_inf(inf_t *, char **);
void ft_free_inf(inf_t *, int);

char *ft_getenv(inf_t *, const char *);
int ft_myenv(inf_t *);
int ft__mysetenv(inf_t *);
int ft_myunsetenv(inf_t *);
int populate_env_list(inf_t *);

char **get_environ(inf_t *);
int ft_unsetenv(inf_t *, char *);
int ft_setenv(inf_t *, char *, char *);

char *get_history_file(inf_t *inf);
int ft_write_history(inf_t *inf);
int ft_read_history(inf_t *inf);
int ft_build_history_list(inf_t *inf, char *buf, int line_count);
int ft_renumber_history(inf_t *inf);

list_t *ft_add_node(list_t **, const char *, int);
list_t *ft_add_node_end(list_t **, const char *, int);
size_t ft_ft_print_list_str(const list_t *);
int node_at_index(list_t **, unsigned int);
void ft_free_list(list_t **);

size_t ft_list_len(const list_t *);
char **ft_list_to_strings(list_t *);
size_t ft_print_list(const list_t *);
list_t *ft_node_starts(list_t *, char *, char);
ssize_t ft_get_node_index(list_t *, list_t *);

int is_chain(inf_t *, char *, size_t *);
void ft_check_chain(inf_t *, char *, size_t *, size_t, size_t);
int ft_replace_alias(inf_t *);
int ft_replace_vars(inf_t *);
int ft_replace_string(char **, char *);

#endif
