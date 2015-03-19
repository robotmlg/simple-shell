CFLAGS = -Wall
CC=gcc

all: shell

debug: CFLAGS += -g
debug: alloc

shell: shell.c shell.h
	$(CC) $(CFLAGS) -o shell shell.c

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

