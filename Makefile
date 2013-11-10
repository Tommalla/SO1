CFLAGS+=-O2 -Wall --pedantic -g

.PHONY: all clean release

all: manager executor

manager: manager.c common.o err.o
	gcc $(CFLAGS) common.o err.o manager.c -o manager

executor: executor.c common.o
	gcc $(CFLAGS) common.o executor.c -o executor

common.o: common.h common.c
	gcc $(CFLAGS) -c common.c -o common.o

err.o: err.h err.c
	gcc $(CFLAGS) -c err.c -o err.o

clean:
	rm -rf *.o

release: all clean
	rm -rf release
	mkdir release
	cp -f *.c release
	cp -f *.h release
	cp -f Makefile release