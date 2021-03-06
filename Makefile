#
# Simple shell with pipes
# Written by Matt Goldman
# Rutgers University 198:416:04
# Last updated 2015-03-22
#
# Makefile: make instructions for shell, with rule for debugging
#

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

tar : shell.c shell.h Makefile builtins.c builtins.h
	mkdir mgoldman_assgn5;\
  cp *.c mgoldman_assgn5;\
  cp *.h mgoldman_assgn5;\
  cp Makefile mgoldman_assgn5;\
  cp README mgoldman_assgn5;\
  tar cvf mgoldman_assgn5.tar mgoldman_assgn5;\
  rm -rf mgoldman_assgn5

