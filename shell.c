#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include "shell.h"

int main(int argc, char **argv){
  char input_line[MAX_INPUT_LENGTH];
  char *input_toks[MAX_INPUT_TOKENS];
  int i             = 0;

  while(true){
    //print prompt
    if(isatty(0))
      printf("%s ",PROMPT_STR);
    //get input line
    if(fgets(input_line,MAX_INPUT_LENGTH,stdin) == 0)
      return 0;
    //parse input string into tokens
    input_toks[0] = lntok(input_line);
    for(i=1; i<MAX_INPUT_TOKENS; ++i){
      input_toks[i] = lntok(NULL);
    }
#ifdef DEBUG
    for(i=0;i<MAX_INPUT_TOKENS;++i)
      printf("%s\n",input_toks[i]);
#endif
    //parse input into command strings
    //execute commands
    //wait for commands to finish

  }

  return 0;
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
