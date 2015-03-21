#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"

int main(int argc, char **argv){
  command_t *cmd_list = NULL;
  char input_line[MAX_INPUT_LENGTH];
  int i             = 0;

  while(true){
    //print prompt
    if(isatty(0))
      printf("%s ",PROMPT_STR);
    //get input line
    if(fgets(input_line,MAX_INPUT_LENGTH,stdin) == 0)
      return 0;
    //parse input into command strings
    cmd_list = get_cmd_list(input_line);
    if(cmd_list == NULL)
      break;

    //execute commands
    //wait for commands to finish
    destroy_command(cmd_list);
  }

  return 0;
}

command_t *create_command(){
  command_t *head = malloc(sizeof(command_t));
  if(head == NULL)
    return NULL;
  head->argc = 1;
  head->fd_in = 0;
  head->fd_out = 0;
  head->next = NULL;

  return head;
}

void destroy_command(command_t *c){
  if(c == NULL){}
  else if(c->next == NULL){
    free(c);
  }
  else{
    destroy_command(c->next);
    free(c);
  }
}

//parse the input line into tokens
//a token is a quoted string (single or double), a group of non-whitespace
//characters, or a pipe, |
//This works just like strtok, and is largely based on the strtok source code
char *lntok(char *s){
  static char *pipe = "|";
  static char *last = 0;
  static char pflag = 0;//indicate if last string ended with adjacent pipe
  char curr = 0;

  //if input is NULL, we want to reuse the same string
  if(s == NULL){
    s = last;
  }
  //if the last token ended with an adjacent pipe, return a pipe this time
  if(pflag){
    pflag = 0;
    return pipe;
  }
  //skip initial whitespace
  do{
    //if you find the end of the string in the whitespace, you have no tokens
    if((curr = *s++) == '\0')
      return NULL;
  }while(isspace(curr));
  --s;
  curr = *s;
  //if you have a quoted string, find the end quote and return it
  if(curr == '\"' || curr == '\''){
    //skip the quote
    ++s;
    //find the end of the string
    for(last = s; *last != curr; ++last);
    //null terminate it and advance it
    *last++ = '\0';
  }
  //if you have a pipe, return a static pipe string,
  //because pipes don't need whitespace around them
  else if(curr == '|'){
    last = s+1;
    return pipe;
  }
  //if you have a regular string, find the next whitespace
  else{
    //find the end of the string
    for(last = s; !isspace(*last) && *last != '|'; ++last);
    if(*last == '|'){
      pflag = 1;
    }
    //null terminate it and advance it
    *last++ = '\0';
  }

  return s;
}

//parse the input line into command and argument arrays
command_t *get_cmd_list(char *input_line){ 
  command_t *head = NULL;
  command_t *curr = NULL;
  int i                   = 0;

  head = create_command();
  if(head == NULL)
    return NULL;
  curr = head;

  //parse input string into tokens, put each token into the current argument array
  curr->argv[0] = lntok(input_line);
  for(i=1; curr->argv[i-1] != 0 && i<MAX_INPUT_TOKENS; ++i){
    curr->argv[i] = lntok(NULL);
    if(curr->argv[i] == NULL)
      return head;
    //if you get a pipe, make a new command
    else if(strcmp(curr->argv[i],"|") == 0){
      curr->argv[i] = 0;
      curr->next = create_command();
      if(curr->next == NULL)
        return NULL;
      curr = curr->next;
    }
    //otherwise, just put the string in the array and continue
    else{
      ++curr->argc;
    }
  }
  return head;
}
