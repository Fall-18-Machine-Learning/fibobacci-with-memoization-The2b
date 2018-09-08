/**
 * @author Thomas Lenz <thomas.lenz96@gmail.com> AS The2b
 * @date 08 September 2018
 * @project Fibonacci Memoization
 * @file fib.h
 *
 * Manages includes, provides prototypes for functions, and defines fibStruct
 */

#ifndef _THE2B_FIB_H
#define _THE2B_FIB_H

#include "config.h"

// I'm not going to add shields here since if you don't have these, you won't be able to compile anything
#include <stdlib.h>		// malloc
#include <stdio.h>		// printf, sprintf, fgets
#include <string.h>		// strcmp
#include <errno.h>		// errno

#ifdef HAVE_UINTMAX_T
# ifdef HAVE_STDINT_H
#  include <stdint.h>		// Possibly uintmax_t
# endif /* HAVE_STDINT_H */
# ifdef HAVE_INTTYPES_H
#  include <inttypes.h>		// strtoumax, possibly uintmax_t
# endif
#endif /* HAVE_UINTMAX_T */

#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>	// readline
#endif /* HAS_READLINE_READLINE_H */ // No need for an else since the alternative, stdio.h, is already included regardless

#ifdef HAVE_GMP_H
#include <gmp.h>
#endif /* HAS_GMP_H */

struct fibStruct {
    size_t size;					    // The number of fibonacci values we want to calculate. Has an unenforced cap of 94, afterwhich overflows occur
    ELEM_T* fib;					    // The array which holds the calculated values. Pointer typing detemined at compile-time
};

static struct fibStruct* createFibStruct(size_t fibSize);   // Allocates memory for the struct, returns the pointer
static void populateFibStruct(struct fibStruct*);	    // Populates the fibStruct's array with fibonacci data

struct fibStruct* buildFibStruct(size_t fibSize);	    // Executes both of the above functions, and returns the resulting allocated and populated fibStruct

void destroyFibStruct(struct fibStruct*);		    // Frees all memory tied to a fibStruct

int fibValue(struct fibStruct*, size_t index, char** val);  // Find the index-th value in the populated fibonacci sequence. Returns NULL if you go our of bounds.

static int outputFibValues(struct fibStruct*);		    // A debug function which prints out every fibonacci value in the struct after being fully populated

static int strToIntErrors();				    // Checks errno for errors (overflows and invalid inputs) when converting input strings to ints
static inline int eofNewlineChecker(char* inStr);	    // Checks the input string for blank lines and EOF

#endif /* _THE2B_FIB_H */
