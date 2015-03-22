#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtins.h"

builtin_t bi_table[] = {
  {"exit"     , my_exit},
  {"cd"       , cd},
  {0          , 0},
  };

int exec_bi(int argc,char **argv){
  builtin_t *b = 0;
  //search for built-in in table
  for(b = bi_table; b->name != 0; ++b){
    if(strcmp(b->name,argv[0]) == 0)
      return b->func(argc,argv);
  }
  return -1;
}

int my_exit(int argc,char **argv){
  long val = 0;
  //fprintf(stderr,"exit\n");
  if(argc == 1){
    exit(0);
  }
  else{
    val = strtol(argv[1],NULL,10);
    if(errno){
      perror("exit");
      exit(255);
    }
    exit(val);
  }
  return 0;
}
int cd(int argc,char **argv){
  fprintf(stderr,"cd\n");
  return 0;
}
