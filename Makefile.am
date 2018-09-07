bin_PROGRAMS = fib
fib_SOURCES = fib.c fib.h config.h stdlib.h stdio.h string.h errno.h
AM_LDFLAGS =

if WITH_GMP
AM_LDFLAGS += -lgmp
fib_SOURCES += gmp.h
endif # WITH_GMP

if HAVE_STDINT_H
fib_SOURCES += stdint.h
endif # HAVE_STDINT_H

if HAVE_INTTYPES_H
fib_SOURCES += inttypes.h
endif # HAVE_INTTYPES_H

if WITH_READLINE_READLINE_H
AM_LDFLAGS += -lreadline
fib_SOURCES += readline/readline.h
else
fib_SOURCES += unistd.h
endif # WITH_READLINE_READLINE_H
