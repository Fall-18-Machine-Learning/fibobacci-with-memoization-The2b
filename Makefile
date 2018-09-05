# Define the compiler
CC = gcc
CFLAGS = -Wall
LIBS = -lgmp -lreadline
INCLUDES =
AUTOHEADER = autoheader
AUTOCONF = autoconf
CONFIGUREFLAGS =
CONFIGURE = ./configure

TARGET = fibonacci-memoization

all: ${TARGET}

${TARGET}: fib.o
	${CC} ${CFLAGS} ${LIBS} ${INCLUDES} -o ${TARGET} fib.c

config.h.in: configure.ac
	${AUTOHEADER}

${CONFIGURE}: configure.ac

config.h: config.h.in  configure.ac
	${AUTOCONF}
	${CONFIGURE}

fib.o: fib.c fib.h config.h
	${CC} ${CFLAGS} ${LIBS} ${INCLUDES} -c fib.c

clean:
	${RM} ${TARGET} fib.o

distclean:
	make clean
	${RM} -r config.log config.status config.h config.h.in autom4te.cache ${CONFIGURE}
