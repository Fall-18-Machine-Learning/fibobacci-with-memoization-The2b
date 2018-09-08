/**
 * @author Thomas Lenz <thomas.lenz96@gmail.com> AS The2b
 * @date 08 September 2018
 * @project Fibonacci Memoization
 * @file fib.c
 * 
 * Handles most of the fibonacci memoization. Anything that isn't handled here is abstracted away from it and macro'd in by configure
 */

#include "fib.h"

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-label"
#define true 1
#define false 0

#define EOF_VAL 2
#define NEWLINE_VAL 1

/**
 * Creates and populates a fibonacci structure
 * This is the only function you should call in order to create such a structure
  // read only gives us a raw input. Add the null-terminator so that we can actually use the string in the furure*
 * @param size_t size
 * @returns struct fibStruct*
 */
struct fibStruct* buildFibStruct(size_t size) {
    struct fibStruct* sFib = createFibStruct(size);
    populateFibStruct(sFib);

    return sFib;
}

/**
 * Release the memory associated with a fibStruct
 *
 * @param struct fibStruct*
 */
void destroyFibStruct(struct fibStruct* sFib) {
    // Destroy all of the memory allocated for fib
#ifdef WITH_GMP_H
    // With GMP, these objects need to be cleared by GMP. If we're not using it, they're just ints and we can just free the memory
    for(size_t index = 0; index < sFib->size; index++) {
	mpz_clear(sFib->fib[index]);
    }
#endif /* HAVE_GMP_H */
    // Free the memory allocated for their pointers
    free(sFib->fib);

    // And free the memory of the struct itself
    free(sFib);

    return;
}

/**
 * Get the index-th fibonacci value from a fibonacci structure
 * This function should be processed with either strtol (or a similar function) or using a bignum library. The latter is preferred, to prevent overflows
 *
 * @param struct fibStruct*
 * @param size_t index
 * @returns char* fibVal
 */
int fibValue(struct fibStruct* sFib, size_t index, char** val) {
    // Since we're using unsigned long long ints, this can be an error checker, since you really shouldn't need to check the first index, and there isn't any other value we can use
    if(index >= sFib->size) {
	return -1;
    }

    else {
	// Defined at compilation time in config.h
	FIBGET(sFib->fib[index],val);
    }
    
    return 0;
}

/**
 * Allocate memory for and populate a fibStruct
 *
 * This function should not be called by users; Instead, call the buildFibStruct function
 *
 * @param size_t size
 * @returns struct fibStruct*
 */
static struct fibStruct* createFibStruct(size_t size) {
    // Defined in fib.h
    struct fibStruct* sFib = malloc(sizeof(struct fibStruct));

    // The number of indexes we want to store
    sFib->size = size;
    
    // This is where we are storing the fibonacci values
    // Allocate the structs' memory blocks
    sFib->fib = malloc(sizeof(ELEM_T) * size);

    return sFib;
}

/**
 * Populate a fibStruct with fibonacci values
 *
 * This should not be called by users. Instead, call the buildFibStruct function
 *
 * @param struct fibStruct*
 */
static void populateFibStruct(struct fibStruct* sFib) {
    size_t size = sFib->size;

    if(size > 1) {
	goto size1;
    }

    else if(size > 0) {
	goto size0;
    }

    // If we somehow get a size 0, handle it
    else if(size == 0) {
	return;
    }

size1:
    // Defined at compilation time in config.h
    FIBSET(sFib->fib[1],1);
    
size0:
    // Defined at compilation time in config.h
    FIBSET(sFib->fib[0],0);

    // This will automatically be skipped if the size is <= 2, so don't waste time checking with an extra if
    for(size_t index = 2; index < size; index++) {
	FIBADD(sFib->fib[index], sFib->fib[index - 1], sFib->fib[index - 2]);
    }

    return;
}

/**
 * This is a debug function used to verify that the fibonacci values are behaving correctly
 * @DEBUG
 *
 * @param struct fibStruct*
 * @return int error
 */
static int outputFibValues(struct fibStruct* sFib) {
    char* fibVal;
    for(size_t index = 1; index < sFib->size; index++) {
	if(fibValue(sFib,index,&fibVal) == 0) {
	    printf("%zu: %s\n",index,fibVal);
	}
	else {
	    printf("Error in outputFibValues; Attempting to reach an inappropriate index value");
	    return -1;
	}
    }

    return 0;
}

/**
 * This function checks for errors in processing strings/char arrays into ints
 * Returns 0 if errno is appropriate, or -1 if a relevant error is caught
 *
 * @return int error
 */
static int strToIntErrors() {
    // Error handling
    if(errno == ERANGE) {
	printf("That value is too big for a 64-bit unsigned integer. Please input a smaller value\n");
	return -1;
    }
    
    else if(errno == EINVAL) {
	// In some implentations, this can occur if we get an invalid input. In all standard implementations, this will occur if there's an invalid base value. Since the base value is hard coded, let's assume this is the former.
	printf("Invalid input. This input accepts only positive integers.\n");
	return -1;
    }

    return 0;
}

/**
 * Checks the input for newlines or EOF. Returns 0 if neither are present, 1 if only a newline is present, and 2 if only an EOF is present
 * Due to the relative complexity of these statements, we're using conditional compilation instead of a macro, and inline it for a similar effect to macros
 *
 * @param char* str
 * @returns int specialCaseVal
 */
static inline int eofNewlineChecker(char* str) {
#ifdef WITH_READLINE_READLINE_H
    if(str == NULL) {
	return EOF_VAL;
    }

    if(strcmp(str,"") == 0) {
	return NEWLINE_VAL;
    }

    return 0;
#else
    if(strcmp(str,"") == 0) {
	return EOF_VAL;
    }

    if(strcmp(str,"\n") == 0) {
	return NEWLINE_VAL;
    }

    return 0;
#endif
}

int main() {
    size_t calcSize;

start:
    ;
    // Error checker
    int error = 0;
    do {

	// Figure out how many fibonacci numbers to calc from user
	const char* HOW_MANY_FIB_Q = "How many fibonacci numbers would you like to calculate, including 0? ";
	char* sizeStr;

	// Read from stdin how many fibonacci numbers to calc
	// This is defined at compilation time. When configure decides whether or not to use GNU Readline, it puts a macro into config.h with the proper command.
	READ_STDIN(&sizeStr,HOW_MANY_FIB_Q);

	// Check for EOF or newline. Handle each situation appropriately.
	int eofNewline = eofNewlineChecker(sizeStr);

	if(eofNewline == EOF_VAL) {
	    printf("\n");
	    exit(0);
	}

	else if (eofNewline == NEWLINE_VAL) {
	    free(sizeStr);
	    continue;
	}
	
	// Try to convert the string to a usable value
	calcSize = strtoumax(sizeStr,NULL,10);

	// Catch errors
	error = strToIntErrors();

	// I need an additional error check here in case non-alphanumeric characters are input, since errno won't catch it in all implementations
	if(calcSize == NULL) { // Null will automatically cast to an int
	    printf("Invalid value. This input accepts positive integers only\n");
	    error = -1;
	}

	free(sizeStr);
    } while(error == -1);

    // Create and memoize a fibonacci structure
    struct fibStruct* sFib = buildFibStruct(calcSize);

    //outputFibValues(sFib); // @DEBUG

    while(true) {
	printf("Which fibonacci index would you like? Must be between 0 and %zd: ",sFib->size - 1);
	char* str;
	
	// Defined at compilation-time in config.h
	READ_STDIN(&str,"");

	// Check for EOF or newline. Handle each situation appropriately.
	int eofNewline = eofNewlineChecker(str);

	if(eofNewline == EOF_VAL) {
	    printf("\n");
	    break;
	}

	else if (eofNewline == NEWLINE_VAL) {
	    free(str);
	    continue;
	}

	uintmax_t input = strtoumax(str,NULL,10);

	// This error function checks overflows for the size value as well as invalid inputs, prints the proper message, and returns -1 if there's an error, or 0 if there's not. If it returns something other than 0, jump out
	if(strToIntErrors() != 0) {
	    free(str);
	    continue;
	}

	// Protect against buffer overflows of the pre-calc'd fibonacci numbers
	if(input >= sFib->size) {
	    printf("Input value must be between 0 and %zu\n",sFib->size - 1);
	    free(str);
	    continue;
	}

	// If we are still running, we have a non-empty value
	char* fibVal; // This works with or without GMP, since it will be properly written over by GMPs functions if need be
	if(fibValue(sFib,input,&fibVal) == 0) {
	    // Output
	    printf("%s\n",fibVal);
	}

	free(str);
    }
    
    // Free our memory
    destroyFibStruct(sFib);

    // Done!!
    return 0;
}
