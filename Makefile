CFLAGS+=-Wall --pedantic

ifeq ($(debug),1)
	CFLAGS+= -g -DDEBUG
else
	CFLAGS+= -O2
endif

all: manager executor

manager: manager.c common.o err.o
	gcc $(CFLAGS) common.o err.o manager.c -o manager

executor: executor.c common.o err.o
	gcc $(CFLAGS) common.o err.o executor.c -o executor

common.o: common.h common.c
	gcc $(CFLAGS) -c common.c -o common.o

err.o: err.h err.c
	gcc $(CFLAGS) -c err.c -o err.o

clean:
	rm -rf *.o
	rm -rf manager executor

release: all clean
	rm -rf release
	mkdir release
	cp -f *.c release
	cp -f *.h release
	cp -f Makefile release

.PHONY: all clean release