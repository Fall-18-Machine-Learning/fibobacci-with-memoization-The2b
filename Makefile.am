bin_PROGRAMS = fib
fib_SOURCES = fib.c fib.h config.h stdlib.h stdio.h string.h errno.h
AM_LDFLAGS =

if WITH_GMP
fib_SOURCES += gmp.h
AM_LDFLAGS += -lgmp
endif

if HAVE_STDINT_H
fib_SOURCES += stdint.h
endif

if HAVE_INTTYPES_H
fib_SOURCES += inttypes.h
endif

# @TODO Make this optional
if HAVE_READLINE_READLINE_H
AM_LDFLAGS += -lreadline
endif
