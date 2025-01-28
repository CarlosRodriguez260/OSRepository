#include "FillDate.h"
#include "math.h"
#include <stdio.h>
#include "string.h"

void FillDate(long second, int* day, int* month, int* year){
    // day = 1
    // month = 1
    // year = 1970

    // 1 year = 31,536,000 seconds
    // 1 month = 2,628,000 seconds
    // 1 day = 86,400 second

    int day1 = *(&day);
    int month1 = *(&month);
    int year1 = *(&year);

    if(second<86400){printf("January 1,1970"); printf("\n"); return;}

    while(second>=31536000){
        year1++;
        second = second - 31536000;
    }
    while(second>=2628000){
        month1++;
        second = second - 2628000;
    }
    while(second>=86400)
    {
        day1++;
        second = second - 86400;
    }

    if(month1==1){printf("January %d,%d\n",day1,year1);}
    if(month1==2){
        if(day1>=29 && (year1%4==0 || year1%400==0)){day1=29;} 
        else if(day1>=28 && (year1%4!=0 || year1%100==0)){day1=28;}
        printf("February %d,%d\n",day1,year1);
    }
    if(month1==3){printf("March %d,%d\n",day1,year1);}
    if(month1==4){printf("April%d,%d\n",day1,year1);}
    if(month1==5){printf("May %d,%d\n",day1,year1);}
    if(month1==6){printf("June %d,%d\n",day1,year1);}
    if(month1==7){printf("July %d,%d\n",day1,year1);}
    if(month1==8){printf("August %d,%d\n",day1,year1);}
    if(month1==9){printf("September %d,%d\n",day1,year1);}
    if(month1==10){printf("October %d,%d\n",day1,year1);}
    if(month1==11){printf("November %d,%d\n",day1,year1);}
    if(month1==12){printf("December %d,%d\n",day1,year1);}
}