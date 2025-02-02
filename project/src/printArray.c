#include "header.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Receives two character arrays and prints them out with puts().
 * 
 * 
 * 
 * @param base Points to the unmodified character array
 * @param mod Points to the modified character array
 * @return Prints out both char arrays on screen
 */

void PrintArray(char * base, char * mod){
    printf("Unaltered String: ");
    puts(base);
    printf("\nAltered String: ");
    puts(mod);
}