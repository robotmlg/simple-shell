#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "shell.h"

int main(int argc, char **argv){
  char input_line[MAX_INPUT_LENGTH];

  while(true){
    //print prompt
    printf("%s ",PROMPT_STR);
    //get input line
    fgets(input_line,MAX_INPUT_LENGTH,stdin);
#ifdef DEBUG
    printf("%s",input_line);
#endif
    //parse input into command strings
    //execute commands
    //wait for commands to finish

  }

  return 0;
}
