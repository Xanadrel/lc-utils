##
## LC utils Makefile
##

CFLAGS += -Wall -W -pipe -O2 -std=c99

all: clean release

release: linux windows
	$(STRIP_LINUX)   *.bin
	$(STRIP_WINDOWS) *.exe
	mv *.bin ./bin
	mv *.exe ./bin

clean:
	rm -f ./bin/*.bin ./bin/*.exe ./bin/*.pl
	rm -f *.bin *.exe

##
## LINUX
##

CC            ?= gcc
CC_LINUX       = $(CC)
STRIP_LINUX    = strip
CFLAGS_LINUX   = $(CFLAGS) -D_LINUX
LDFLAGS_LINUX  = $(LDFLAGS)

linux:
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o alarm.bin src/alarm.c
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o dict.bin src/dict.c
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o leftlist.bin src/leftlist.c
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o sed_cut_coffee.bin src/sed_cut_coffee.c
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o dgrep2.bin src/dgrep2.c	
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o metagen.bin src/metagen.c
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o metabuild.bin src/metabuild.c
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o rli2_len.bin src/rli2_len.c
	${CC_LINUX} ${CFLAGS_LINUX} ${LDFLAGS_LINUX} -o cmp_rli2.bin src/cmp_rli2.c
	
##
## WINDOWS
##

CC_WINDOWS     = x86_64-w64-mingw32-gcc
STRIP_WINDOWS  = x86_64-w64-mingw32-strip
CFLAGS_WINDOWS = $(CFLAGS) -D_WINDOWS -I/usr/i686-w64-mingw32/include/

windows:
	# ${CC_WINDOWS} ${CFLAGS_WINDOWS} -o alarm.exe src/alarm.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o dict.exe src/dict.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o leftlist.exe src/leftlist.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o sed_cut_coffee.exe src/sed_cut_coffee.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o dgrep2.exe src/dgrep2.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o metagen.exe src/metagen.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o metabuild.exe src/metabuild.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o rli2_len.exe src/rli2_len.c
	${CC_WINDOWS} ${CFLAGS_WINDOWS} -o cmp_rli2.exe src/cmp_rli2.c
	