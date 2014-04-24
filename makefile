
INCLUDE_DIR=include

CPPFLAGS=-I$(INCLUDE_DIR)

all: libmthread.a

libmthread.a: mthread.o linkedlist.o
	@ar crs ./lib/libmthread.a ./bin/mthread.o ./bin/linkedlist.o
	@echo "\nlibmthread compiled.\n"

mthread.o: src/mthread.c ./include/mthread.h
	@if test -d lib; then rm -r lib; fi
	@mkdir lib
	@if test -d bin; then rm -r bin; fi
	@mkdir bin
	@gcc -c -g -Wall src/mthread.c -o ./bin/mthread.o $(CPPFLAGS)

linkedlist.o: src/linkedlist.c ./include/linkedlist.h
	@gcc -c -g -Wall ./src/linkedlist.c -o ./bin/linkedlist.o  $(CPPFLAGS)
