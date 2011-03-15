CC=gcc
CFLAGS=-O0 -g -D _GLOBALCONF -lcunit -fprofile-arcs -ftest-coverage

all: test-package test-control test-config

#package
test-package: test-package.o package.o ../src/package.o
	gcc $(CFLAGS) -o test-package $^

test-package.o: test-package.c test-package.h package.h
	gcc $(CFLAGS) -c test-package.c -o test-package.o

package.o: package.c package.h test-package.h
	gcc $(CFLAGS) -c package.c -o package.o

../src/package.o: ../src/package.c ../src/package.h
	gcc $(CFLAGS) -c ../src/package.c -o ../src/package.o

#control
test-control: test-control.o control.o ../src/control.o ../src/package.o
	gcc $(CFLAGS) -o test-control $^

test-control.o: test-control.c test-control.h control.h
	gcc $(CFLAGS) -c test-control.c -o test-control.o

control.o: control.c control.h test-control.h
	gcc $(CFLAGS) -c control.c -o control.o

../src/control.o: ../src/control.c ../src/control.h
	gcc $(CFLAGS) -c ../src/control.c -o ../src/control.o

#config
test-config: test-config.o config.o ../src/config.o
	gcc $(CFLAGS) -o test-config $^

test-config.o: test-config.c test-config.h config.h
	gcc $(CFLAGS) -c test-config.c -o test-config.o

config.o: config.c config.h test-config.h
	gcc $(CFLAGS) -c config.c -o config.o

../src/config.o: ../src/config.c ../src/config.h
	gcc $(CFLAGS) -c ../src/config.c -o ../src/config.o


clean:
	rm -f *.o ../src/*.o test-package *.gcda ../src/*.gcda
