CFLAGS = -Wall
CC=gcc

all: shell

debug: CFLAGS += -g -DDEBUG
debug: shell

shell: shell.c shell.h builtins.o
	$(CC) $(CFLAGS) -o shell shell.c builtins.o

builtins.o: builtins.c builtins.h
	$(CC) $(CFLAGS) -c -o builtins.o builtins.c

clean:
	rm shell;\
  rm -rf *.dSYM;\
	rm *.o;\
  rm *.tar;

tar : shell.c shell.h Makefile
	mkdir mgoldman_proj1;\
  cp *.c mgoldman_proj1;\
  cp *.h mgoldman_proj1;\
  cp Makefile mgoldman_proj1;\
  cp README mgoldman_proj1;\
  tar cvf mgoldman_proj1.tar mgoldman_proj1;\
  rm -rf mgoldman_proj1

