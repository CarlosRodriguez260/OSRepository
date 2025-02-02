#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

/**
 * @brief Receives a pointer "base" that points to a char array filled with certain characters. We then
 *        iterate through this char array and randomly choose to uppercase them. This modified char array
 *        gets passed to the pointer of "mod", which points to another char array.
 * 
 * 
 * 
 * @param base Points to the unmodified character array
 * @param mod Points to where we still store the modified char array
 * @return Nothing, only modifies the values pointed to by the parameters
 */

void UpperRand(char * base, char * mod){
    int random;
    for(int i = 0; base[i] != '\0'; i++){
        char temp = base[i];
        random = rand();
        // We only uppercase the letter if the random number is divisible by 2
        if(random%2==0){
            temp = toupper(base[i]);
        }
        mod[i] = temp;
    }
}