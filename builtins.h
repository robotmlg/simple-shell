/*
 * Simple shell with pipes
 * Written by Matt Goldman
 * Rutgers University 198:416:04
 * Last updated 2015-03-22
 *
 * builtins.h: Built-in function library declarations
 */
#ifndef _BUILTINS_H_
#define _BUILTINS_H_

struct builtin_t{
  char *name;
  int (*func)(int,char**);
};
typedef struct builtin_t builtin_t;

int exec_bi(int,char **);

//prototypes for builtin functions
int my_exit(int,char **);
int cd(int, char **);




#endif
