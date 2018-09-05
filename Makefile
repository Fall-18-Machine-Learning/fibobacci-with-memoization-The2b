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

config.in: configure.ac
	${AUTOHEADER}

configure: configure.ac config.in
	${AUTOCONF}

config.h: config.in  configure.ac configure
	${CONFIGURE}

fib.o: fib.c fib.h config.h
	${CC} ${CFLAGS} ${LIBS} ${INCLUDES} -c fib.c

clean:
	${RM} ${TARGET} fib.o

distclean:
	make clean
	${RM} -r config.log config.status config.h config.in autom4te.cache ${CONFIGURE}
