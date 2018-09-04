/**
 *
 */

#include <stdlib.h>	// malloc, rand
#include <stdio.h>	// printf
#include <time.h>	// time
#include <string.h>	// strcmp

#include "fib.h"

typedef int bool;
#define true 1
#define false 0

// Allocate memory for and populate a fibStruct
struct fibStruct* createFibStruct(size_t size) {
    // Allocate memory for our structure. Currently, this is at least 2 bytes for the size value (at least 16 bits per C99), and 4 (32-bit CPU) to 8 (64-bit CPU) bytes for the pointer address, in most cases
    // Defined in fib.h
    struct fibStruct* sFib = malloc(sizeof(struct fibStruct));

    // The number of indexes we want to store. While it is not enforced, there is a hard cap of 94. Any more will overflow the 64-bit int.
    sFib->size = size;
    
    // This is where we are storing the fibonacci values
    // Allocate the correct number of bytes for the platform (8 per index for 64 bit, 4 for 32). GCC/MinGW will decide what that is.
    sFib->fib = malloc(sizeof(unsigned long long int) * size);

    return sFib;
}

// Release the memory associated with a fibStruct, recursively
void destroyFibStruct(struct fibStruct* sFib) {
    free(sFib->fib);
    free(sFib);
    return;
}

// Creates and populates a fibStruct
struct fibStruct* buildFibStruct(size_t size) {
    struct fibStruct* sFib = createFibStruct(size);
    populateFibStruct(sFib);

    return sFib;
}

// Populate a fibStruct
void populateFibStruct(struct fibStruct* sFib) {
    size_t size = sFib->size;

    if(size > 1) {
	goto size1;
    }

    else if(size > 0) {
	goto size0;
    }

    // If someone's dumb enough to put in 0, handle it
    else if(size == 0) {
	return;
    }

size1:
    // The ULL suffix indicates that the constant should be stored as an unsigned 64-bit int
    sFib->fib[1] = 1ULL;
    
size0:
    sFib->fib[0] = 0ULL;

    // This will automatically be skipped if the size is <= 2, so don't waste time checking with an if
    for(int index = 2; index < size; index++) {
	sFib->fib[index] = sFib->fib[index - 1] + sFib->fib[index - 2];
    }

    return;
}

// Get the index-th fibonacci value from a fibonacci structure
// Has size protection
unsigned long long int fibValue(struct fibStruct* sFib, size_t index) {
    // Since we're using unsigned long long ints, this can be an error checker, since you really shouldn't need to check the first index, and there isn't any other value we can use
    if(index >= sFib->size) {
	return NULL;
    }

    else {
	return sFib->fib[index];
    }

}

int main() {
    // Any higher and we overflow. This is a 64-bit unsigned int, so not much more we can do without far more complicated structures I don't want to deal with
    const int VAL = 94;
    char str[100];

    // Create and populate our structure
    struct fibStruct* sFib = buildFibStruct(VAL);

    while(true) {

	printf("Which fibonacci index would you like? Must be between 1 and %d: ",sFib->size - 1);
	if(scanf("%s",&str) == EOF) {
	    printf("\n");
	    destroyFibStruct(sFib);
	    exit(0);
	}
	
	int input = (int)strtol(str,NULL,10);

	if(input > sFib->size - 1 || input == 0) {
	    printf("Input value must be between 1 and %d\n",sFib->size - 1);
	    continue;
	}

	unsigned long long int fibVal = fibValue(sFib,input);

	printf("%llu\n",fibVal);
    }

    // Release the memory. If we were using this later on to cache the values, we would do this once the structure is no longer useful.
    destroyFibStruct(sFib);

    return 0;
}
