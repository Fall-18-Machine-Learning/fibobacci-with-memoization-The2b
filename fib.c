/**
 *
 */

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
    // Allocate the structs' memory boxes
    sFib->fib = malloc(sizeof(mpz_t) * size);

    return sFib;
}

// Release the memory associated with a fibStruct, recursively
void destroyFibStruct(struct fibStruct* sFib) {
    // Destroy all of the mp structures
    for(int index = 0; index < sFib->size; index++) {
	mpz_clear(sFib->fib[index]);
    }

    // Free the memory allocated for their pointers
    free(sFib->fib);

    // And free the memory of the structs
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
// This uses GMP now, which is slower but allows for more fibonacci values.
void populateFibStruct(struct fibStruct* sFib) {
    size_t size = sFib->size;

    if(size > 1) {
	goto size1;
    }

    else if(size > 0) {
	goto size0;
    }

    // If someone's silly enough to put in 0, handle it
    else if(size == 0) {
	return;
    }

size1:
    mpz_init_set_ui(sFib->fib[1],1);
    
size0:
    mpz_init(sFib->fib[0]); // This sets the value of a to 0, so we don't need to do any re-assigning

    // This will automatically be skipped if the size is <= 2, so don't waste time checking with an if
    for(int index = 2; index < size; index++) {
	mpz_init(sFib->fib[index]);
	mpz_add(sFib->fib[index], sFib->fib[index - 1], sFib->fib[index - 2]);
    }

    return;
}

// Get the index-th fibonacci value from a fibonacci structure
// Has size protection
char* fibValue(struct fibStruct* sFib, size_t index) {
    // Since we're using unsigned long long ints, this can be an error checker, since you really shouldn't need to check the first index, and there isn't any other value we can use
    if(index >= sFib->size) {
	return NULL;
    }

    else {
	char* str = mpz_get_str(NULL,10,sFib->fib[index]);
	return str;    
    }

}

void outputFibValues(struct fibStruct* sFib) {
    for(int index = 1; index < sFib->size; index++) {
	printf("%d: %s\n",index,fibValue(sFib,index));
    }
}

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
    unsigned long long int calcSize;

start:
    ;
    // Error checker
    int error = 0;
    do {
	// Figure out how many fibonacci numbers to calc from user
	char* sizeStr = readline("How many fibonacci numbers would you like to calculate, including 0? ");

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
	calcSize = strtoull(sizeStr,NULL,10);

	error = strToIntErrors();

	if(calcSize == 0) {
	    printf("Invalid value. This input accepts positive integers only\n");
	    error = -1;
	}
    } while(error == -1);

    struct fibStruct* sFib = buildFibStruct(calcSize);

    //outputFibValues(sFib); // @DEBUG

    while(true) {
	printf("Which fibonacci index would you like? Must be between 0 and %d: ",sFib->size - 1);
	char* str = readline(NULL);

	// readline returns the empty string on a newline w/o data, and NULL on EOF
	// Handle each situation appropriately
	if(str == NULL) {
	    printf("\n");
	    destroyFibStruct(sFib);
	    exit(0);
	}

	else if (strcmp(str,"") == 0) {
	    continue;
	}
	
	unsigned long long int input = strtoull(str,NULL,10);

	if(input >= sFib->size) {
	    printf("Input value must be between 0 and %d\n",sFib->size - 1);
	    continue;
	}

	char* fibVal = fibValue(sFib,input);

	printf("%s\n",fibVal);
    }

    // Release the memory. If we were using this later on to cache the values, we would do this once the structure is no longer useful.
    destroyFibStruct(sFib);

    return 0;
}
