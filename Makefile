##
## LC tools Makefile
##

CFLAGS = -Wall -W -pipe -O3

CC  = gcc

all: clean release

clean:
	rm -f bin/*.bin
  
release:
	${CC} ${CFLAGS} -o bin/alarm.bin src/alarm.c
	${CC} ${CFLAGS} -o bin/dict.bin src/dict.c
	${CC} ${CFLAGS} -o bin/leftlist.bin src/leftlist.c
	${CC} ${CFLAGS} -o bin/sed_cut_coffee.bin src/sed_cut_coffee.c