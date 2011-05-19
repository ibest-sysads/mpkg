include ../etc/mpkg.conf	

CC=gcc
CFLAGS=-O0 -g -D _GLOBALCONF
OBJS=main.o package.o control.o config.o

all: build_config $(OBJS)
	gcc $(CFLAGS) -o mpkg $(OBJS)

build_config:
	echo "#define MPKG_CONF_FILE \"${MPKG_CONF_DIR}/mpkg.conf\"" > buildConfig.h

package.o: globals.h package.h package.c
	gcc $(CFLAGS) -c package.c -o package.o  
	
control.o: globals.h control.h control.c
	gcc $(CFLAGS) -c control.c -o control.o
	
config.o: config.h config.c
	gcc $(CFLAGS) -c config.c -o config.o

main.o: globals.h main.c
	gcc $(CFLAGS) -c main.c -o main.o

install:
	mkdir -p ${MPKG_HOME}/
	mkdir -p ${MPKG_CONF_DIR}/
	cp ../bin/* ${MPKG_HOME}/
	cp mpkg ${MPKG_HOME}/
	cp -rf ../etc/* ${MPKG_CONF_DIR}/

clean:
	rm -f $(OBJS) mpkg
