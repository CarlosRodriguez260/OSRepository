#include "cosineSeries.h"
#include "math.h"
#include <stdio.h>

int CosineSeries(int init_number, int end_number) { 
    double result = 0;
    for(int i = init_number; i<=end_number; i++) {
        result = result + cos(i);
    }
    printf("The result is %lf\n",result);
    if(result < 0)
    {
        printf("Hence we print -1");
        return -1;
    }
    else if(result >= 0)
    {
        printf("Hence we print 1");
        return 1;
    }
}