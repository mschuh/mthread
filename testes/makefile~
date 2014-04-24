all: hellothread mjoinerror myieldtest

myieldtest: myieldtest.c ../lib/libmthread.a
	@gcc -c -g -Wall myieldtest.c -o ../bin/myieldtest.o
	@gcc -g -o ../bin/myieldtest myieldtest.c -L../lib -lmthread -Wall
	@rm -rf *.o

mjoinerror: mjoinerror.c ../lib/libmthread.a
	@gcc -c -g -Wall mjoinerror.c -o ../bin/mjoinerror.o
	@gcc -g -o ../bin/mjoinerror mjoinerror.c -L../lib -lmthread -Wall
	@rm -rf *.o

hellothread: hellothread.c ../lib/libmthread.a
	@gcc -c -g -Wall hellothread.c -o ../bin/hellothread.o
	@gcc -g -o ../bin/hellothread hellothread.c -L../lib -lmthread -Wall


testealexandre: testealexandre.c ../lib/libmthread.a
	@gcc -c -g -Wall testealexandre.c -o ../bin/testealexandre.o
	@gcc -g -o ../bin/testealexandre testealexandre.c -L../lib -lmthread -Wall
	@rm -rf *.o