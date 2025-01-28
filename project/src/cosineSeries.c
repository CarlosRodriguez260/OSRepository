#include "cosineSeries.h"
#include "math.h"
#include <stdio.h>

/**
 * @brief Calculates the sum of cosines for a range of integers and evaluates the result.
 * 
 * The function is in charge of calculating the sum of the cosine values for all integers in the 
 * inclusive range specified by the parameters 'init_number' and 'end_number'. A result is printed
 * to the console, which reflects what the function returns.
 * 
 * @param init_number Starting number of the range
 * @param end_number End number of the range
 * @return -1 if result is negative, otherwise return 1
 */

int CosineSeries(int init_number, int end_number) { 
    double result = 0;
    for(int i = init_number; i<=end_number; i++) {
        result = result + cos(i);
    }
    printf("The result is %lf\n",result);
    if(result < 0)
    {
        printf("Hence we print -1 \n");
        return -1;
    }
    else if(result >= 0)
    {
        printf("Hence we print 1 \n");
        return 1;
    }
}