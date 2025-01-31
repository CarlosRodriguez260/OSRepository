#include "printArray.h"
#include "math.h"
#include <stdio.h>

/**
 * @brief receives an array and prints them differently according to the second argument(1: char, 2: integer, 3: float). 
 *          If the array must be printed in a char way, the function must use the function puts() as a character string. 
 *          In case the array is to be printed in an integer way, the numbers are separated by commas until it reaches a negative number. 
 *          In case the array must be printed in a float way, each number is printed with three decimal digits and 
 *          separated by semicolons until it reaches a negative number.
 * 
 * 
 * 
 * @param array Pointer that can point to either a char, array of type int or array of type float
 * @param type Distinguishes the type (1 = char, 2 = int, 3 = float)
 * @return Prints out the char or array accordingly
 */

void PrintArray(void* array, int type){
    if(type==1){
        char * result = (char*)array;
        printf("Your string was: ");
        puts(result);
    } // char
    else if(type==2){
        int size = 500;
        for(int i = 0; i<size; i++){
            int val = *((int*)array + i);
            if(val<=0){
                break;
            }
            else{
                if(i!=0){
                    printf(":");
                }
            }
            printf("%d", val);
        }
    } // integer
    else{
        int size = 500;
        for(int i = 0; i<size; i++){
            float val = *((float*)array + i);
            if(val<=0.00000000 || isnan(val)){
                break;
            }
            else{
                if(i!=0){
                    printf(";");
                }
            }
            printf("%.3f", val);
        }
    } // float, rounded to 3 decimal spaces
}