#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"

int main(int argc, char **argv){
  command_t *cmd_list = NULL;
  command_t *curr     = NULL;
  char input_line[MAX_INPUT_LENGTH];
  //int i             = 0;
  int pid           = 0;
  int status        = 0;

  while(true){
    //print prompt
    if(isatty(0))
      printf("%s ",PROMPT_STR);
    //get input line
    if(fgets(input_line,MAX_INPUT_LENGTH,stdin) == 0)
      return 0;
    //parse input into command strings
    cmd_list = get_cmd_list(input_line);
    if(cmd_list == NULL){
      fprintf(stderr,"shell: Error parsing input. Try again.\n");
      continue;
    }
    //run commands
    for(curr = cmd_list; curr != NULL; curr = curr->next){
#ifdef DEBUG
      print_command(curr);
#endif
      run(curr);
    }

    //wait for commands to finish
    while((pid = wait(&status)) != -1){
#ifdef DEBUG
      fprintf(stderr,"Process %d exits with %d.\n",pid,WEXITSTATUS(status));
#endif
    }
    //clean up
    destroy_command(cmd_list);
  }

  return 0;
}

command_t *create_command(){
  command_t *head = malloc(sizeof(command_t));
  if(head == NULL)
    return NULL;
  head->argc = 1;
  if(pipe(head->pipe) != 0){
    perror("pipe");
    return NULL;
  }
  head->pid = 0;
  head->head_flag = 0;
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

void print_command(command_t *c){
  char *s = NULL;
  if(c == NULL)
    return;
  for(s = c->argv[0]; s != 0; ++s)
    printf("%s ",s);
  printf("\n");
}

void run(command_t *c){
  int pid=0;

  if(c==NULL || c->argv[0] == NULL)
    return;

  switch(pid = fork()){
    case -1: //error
      perror("fork");
      exit(1);
      break;
    case 0: //child
      //if you  have a next command, set up the pipe
      if(c->next != NULL){
        dup2(c->next->pipe[PIPE_IN],1);//send stdout to the pipe input
        close(c->next->pipe[PIPE_OUT]);//close the pipe output
      }
      //if you have a previous command, set up that pipe
      if(c->head_flag == 0){
        dup2(c->pipe[PIPE_OUT],0);//get stdin from the pipe
        close(c->pipe[PIPE_IN]);//close the pipe input
      }
      execvp(c->argv[0],c->argv);//execute the command
      perror(c->argv[0]);//catch any errors
      break;
    default: //parent
      //close both ends of the pipe, because we don't need them
      close(c->pipe[PIPE_OUT]);
      close(c->pipe[PIPE_IN]);
      break;
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
  if(s == NULL)
    return NULL;
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
  head->head_flag = 1;

  //parse input string into tokens, put each token into the current argument array
  curr->argv[0] = lntok(input_line);
  for(i=1; /*curr->argv[i-1] != 0 &&*/ i<MAX_INPUT_TOKENS; ++i){
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
      i = 0;
      curr->argv[i] = lntok(NULL);
    }
    //otherwise, just put the string in the array and continue
    else{
      ++curr->argc;
    }
  }
  return head;
}
