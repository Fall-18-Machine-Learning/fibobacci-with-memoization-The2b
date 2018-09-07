/**
 *
 */

#include "fib.h"

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-label"
#define true 1

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
 * Release the memory associated with a fibStruct
 *
 * @param struct fibStruct*
 */
void destroyFibStruct(struct fibStruct* sFib) {
    // Destroy all of the memory allocated for fib
#ifdef HAVE_GMP_H
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
    FIBSET(sFib->fib[1],1);
    
size0:
    FIBSET(sFib->fib[0],0);

    // This will automatically be skipped if the size is <= 2, so don't waste time checking with an extra if
    for(size_t index = 2; index < size; index++) {
	FIBADD(sFib->fib[index], sFib->fib[index - 1], sFib->fib[index - 2]);
    }

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
	FIBGET(sFib->fib[index],val);
    }
    
    return 0;
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
int strToIntErrors() {
    // Error handling
    if(errno == ERANGE) {
	printf("That value is too big for a 64-bit unsigned integer. Please input a smaller value\n");
	return -1;
    }
    
    else if(errno == EINVAL) {
	printf("Invalid value. This input accepts positive integers only\n");
	return -1;
    }

    return 0;
}

int main() {
    size_t calcSize;

start:
    ;
    // Error checker
    int error = 0;
    do {
	// Figure out how many fibonacci numbers to calc from user
#ifdef HAVE_READLINE_READLINE_H
	char* sizeStr = readline("How many fibonacci numbers would you like to calculate, including 0? ");
#else
	char sizeStr[256]; // Again, nobody should overflow this (since the resulting fib sequence would be massive in comparison) and if they do, they would need GNU libraries anyway
	ssize_t strLen = read(STDIN_FILENO, sizeStr, 255);
	sizeStr[strLen] = '\0'; // read only gives us a raw input. Add the null-terminator so that we can actually use the string in the furure
#endif /* HAVE_READLINE_READLINE_H */

	// If it's EOF, exit cleanly
	if(sizeStr == NULL) {
	    printf("\n");
	    exit(0);
	}
	else if(strcmp(sizeStr,"") == 0) {
	    error = -1;
	    continue;
	}
	
	// Try to convert the string to a usable value
	calcSize = strtoumax(sizeStr,NULL,10);

	error = strToIntErrors();

	if(calcSize == 0) {
	    printf("Invalid value. This input accepts positive integers only\n");
	    error = -1;
	}
    } while(error == -1);

    // Create and memoize a fibonacci structure
    struct fibStruct* sFib = buildFibStruct(calcSize);

    //outputFibValues(sFib); // @DEBUG

    while(true) {
	printf("Which fibonacci index would you like? Must be between 0 and %zd: ",sFib->size - 1);
	char* str = readline(NULL);

	// readline returns the empty string on a newline w/o data or w/ invalid data, and NULL on EOF
	// Handle each situation appropriately
	if(str == NULL) {
	    printf("\n");
	    break;
	}

	else if (strcmp(str,"") == 0) {
	    continue;
	}
	
	// If we are still running, we have a non-empty value
	uintmax_t input = strtoumax(str,NULL,10);

	// Protect against buffer overflows
	if(input >= sFib->size) {
	    printf("Input value must be between 0 and %zu\n",sFib->size - 1);
	    continue;
	}

	// Get our value
	char* fibVal; // This works with or without GMP, since it will be properly written over by GMPs functions if need be
	if(fibValue(sFib,input,&fibVal) == 0) {
	    // Output
	    printf("%s\n",fibVal);
	}
    }
    
    // Free our memory
    destroyFibStruct(sFib);

    // Done!!
    return 0;
}
