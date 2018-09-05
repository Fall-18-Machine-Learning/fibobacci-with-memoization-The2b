#ifndef _THE2B_FIB_H
#define _THE2B_FIB_H

#include "config.h"

#include <stdlib.h>		// malloc
#include <stdio.h>		// printf
#include <string.h>		// strcmp
#include <errno.h>		// errno
#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>	// readline
#endif /* HAS_READLINE_READLINE_H */

#ifdef HAVE_GMP_H
#include <gmp.h>
#else
#include <inttypes.h>
#include <stdint.h>
#endif /* HAS_GMP_H */

struct fibStruct {
    size_t size;					    // The number of fibonacci values we want to calculate. Has an unenforced cap of 94, afterwhich overflows occur
    ELEM_T* fib;					    // The array which holds the calculated values
};

static struct fibStruct* createFibStruct(size_t fibSize);   // Allocates memory for the struct, returns the pointer
static void populateFibStruct(struct fibStruct*);	    // Populates the fibStruct's array with fibonacci data
struct fibStruct* buildFibStruct(size_t fibSize);	    // Executes both of the above functions, and returns the resulting allocated and populated fibStruct

void destroyFibStruct(struct fibStruct*);		    // Frees all memory tied to a fibStruct

char* fibValue(struct fibStruct*, size_t index);	    // Find the index-th value in the populated fibonacci sequence. Returns NULL if you go our of bounds.
#endif /* _THE2B_FIB_H */
