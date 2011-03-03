CC=gcc
CFLAGS=-O0 -g -D _GLOBALCONF
OBJS=main.o package.o control.o config.o

all: $(OBJS)
	gcc $(CFLAGS) -o mpkg $(OBJS)

package.o: globals.h package.h package.c
	gcc $(CFLAGS) -c package.c -o package.o  
	
control.o: globals.h control.h control.c
	gcc $(CFLAGS) -c control.c -o control.o
	
config.o: config.h config.c
	gcc $(CFLAGS) -c config.c -o config.o

main.o: globals.h main.c
	gcc $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(OBJS) mpkg
