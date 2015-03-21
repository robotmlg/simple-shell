#ifndef _SHELL_H_
#define _SHELL_H_

#define PIPE_CHAR '|'
#define PROMPT_STR "$"

#define MAX_INPUT_LENGTH 512 //as defined in FAQ
#define MAX_INPUT_TOKENS 50  //As defined by project spec

//struct to hold command information
struct command_t{
  int argc;
  char *argv[MAX_INPUT_TOKENS];
  int fd_in;
  int fd_out;
  struct command_t *next;
};
typedef struct command_t command_t;

command_t *create_command();
void destroy_command(command_t *);

//parse the input line into tokens
char *lntok(char *);

//parse the input line into command structs
command_t *get_cmd_list(char *);
#endif
