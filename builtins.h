#ifndef _BUILTINS_H_
#define _BUILTINS_H_

struct builtin_t{
  char *name;
  int (*f)();
};
typedef struct builtin_t builtin_t;




#endif
