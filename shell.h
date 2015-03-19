#ifndef _SHELL_H_
#define _SHELL_H_

#define PIPE_CHAR '|'
#define PROMPT_STR "$"

#define MAX_INPUT_LENGTH 256 //Seems like a good length
#define MAX_INPUT_TOKENS 50  //As defined by project spec

//parse the input line into tokens
char *lntok(char *);
#endif
