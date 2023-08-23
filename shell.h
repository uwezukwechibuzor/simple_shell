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

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* for convert_number() */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

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
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
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
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf;	  /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT                                                               \
	{                                                                           \
		NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
			0, 0, 0                                                             \
	}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* toem_shloop.c */
int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* toem_parser.c */
int is_command(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_my_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void print_inputs(char *);
int write_to_char(char);
int write_char_fd(char c, int fd);
int prints_an_input_string(char *str, int fd);

/* toem_string.c */
int strlen_length(char *);
int _compare_strang(char *, char *);
char *starts_with_haystack(const char *, const char *);
char *concat_two_strings(char *, char *);

/* toem_string1.c */
char *copy_string(char *, char *);
char *duplicate_string(const char *);
void prints_input_string(char *);
int _putchar(char);

/* toem_exits.c */
char *copy_str(char *, char *, int);
char *concat_string(char *, char *, int);
char *find_char_in_string(char *, char);

/* toem_tokenizer.c */
char **split_string_words(char *, char *);
char **split_string_words_2(char *, char);

/* toem_realloc.c */
char *_ismemset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int is_interactive_mode(info_t *);
int check_delimeter(char, char *);
int check_alpha(int);
int convert_string_to_int(char *);

/* toem_errors1.c */
int convert_int(char *);
void print_error(info_t *, char *);
int print_decimal(int, int);
char *convert_number_(long int, int, int);
void delete_comments(char *);

/* toem_builtin.c */
int _myhelp_(info_t *);
int _change_dir(info_t *);
int change_dir_help(info_t *);

/* toem_builtin1.c */
int _displayhistory(info_t *);
int _myalias_builtin(info_t *);

/*toem_getline.c */
ssize_t get_inputs(info_t *);
int _get_next_line(info_t *, char **, size_t *);
void sig_int_Handlers(int);

/* toem_getinfo.c */
void rm_info(info_t *);
void set_info_(info_t *, char **);
void free_info_fields(info_t *, int);

/* toem_environ.c */
char *get_env(info_t *, const char *);
int _myhistory_(info_t *);
int setenv_variables(info_t *);
int _rm_setenv(info_t *);
int display_env_list(info_t *);

/* toem_getenv.c */
char **fetch_env(info_t *);
int _un_setenv(info_t *, char *);
int _set_env(info_t *, char *, char *);

/* toem_history.c */
char *get_my_history_file(info_t *info);
int write_my_history(info_t *info);
int read_my_history(info_t *info);
int build_my_history_list(info_t *info, char *buf, int linecount);
int renumber_my_history(info_t *info);

/* toem_lists.c */
list_t *add_node_list(list_t **, const char *, int);
list_t *add_node_end_list(list_t **, const char *, int);
size_t print_list_str_(const list_t *);
int rm_node_at_index(list_t **, unsigned int);
void free_all_node_list(list_t **);

/* toem_lists1.c */
size_t _list_len(const list_t *);
char **list_to_strings_(list_t *);
size_t my_print_list(const list_t *);
list_t *node_starts_with_prefix(list_t *, char *, char);
ssize_t get_node_at_index(list_t *, list_t *);

/* toem_vars.c */
int is_separated(info_t *, char *, size_t *);
void verify_chain(info_t *, char *, size_t *, size_t, size_t);
int substitue_alias(info_t *);
int get_new_vars(info_t *);
int change_string(char **, char *);

#endif
