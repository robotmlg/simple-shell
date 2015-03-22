#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include "builtins.h"

builtin_t bi_table[] = {
  {"exit"     , my_exit},
  {"cd"       , cd},
  {0          , 0},
  };

//execute a builtin function
int exec_bi(int argc,char **argv){
  builtin_t *b = 0;

  if(argv[0] == 0)
    return -1;
  //search for built-in in table
  for(b = bi_table; b->name != 0; ++b){
    if(strcmp(b->name,argv[0]) == 0)
      return b->func(argc,argv);
  }
  return -1;
}

//exit from the shell
int my_exit(int argc,char **argv){
  long val = 0;
  //fprintf(stderr,"exit\n");
  if(argc == 1){
    exit(0);
  }
  else{
    errno = 0;
    val = strtol(argv[1],NULL,10);
    if(errno){
      perror("exit: strtol");
      exit(255);
    }
    exit(val);
  }
  return 0;
}

//change the directory
int cd(int argc,char **argv){
  char newpath[MAXPATHLEN];

  memset(newpath,0,MAXPATHLEN*sizeof(char));

  //fprintf(stderr,"cd\n");
  if(argc == 1){
    errno = 0;
    if(chdir(getenv("HOME"))==-1){
      perror("cd");
      return 0;
    }
  }
  else if(argc == 2){
    errno = 0;
    getcwd(newpath,MAXPATHLEN);
    if(errno){
      perror("cd");
      return 0;
    }
    if(argv[1][0] != '/'){
      strcat(newpath,"/");
      strcat(newpath,argv[1]);
    }
    else{
      strcpy(newpath,argv[1]);
    }
    //fprintf(stderr,"%s\n",newpath);
    errno = 0;
    if(chdir(newpath)==-1){
      perror("cd");
      return 0;
    }
  }
  else{
    fprintf(stderr,"cd: too many arguments\n");
    return 0;
  }
  return 1;
}
