#include "FillDate.h"
#include <stdio.h>
#include "string.h"

/**
 * @brief Converts a given number of seconds since January 1, 1970 
 *        into a human-readable date (day, month, year).
 *
 * This function calculates the equivalent date (day, month, and year) 
 * for a given number of seconds since January 1, 1970. It assumes:
 *  - 1 year = 31,536,000 seconds
 *  - 1 month = 2,628,000 seconds
 *  - 1 day = 86,400 seconds
 * 
 * @param second The number of seconds since January 1, 1970.
 * @param day Pointer to an integer that stores the initial value of the first day. Value is always equal to 1.
 * @param month Pointer to an integer that stores the initial value representing the first month, January. Value is always equal to 1.
 * @param year Pointer to an integer that stores the computed year.
 * @return The appropiate date after adding the seconds in format of (Month Day, Year) 
 *  - (NOTE: Not an actual return, the date is printed).
*/

void FillDate(long second, int* day, int* month, int* year){
    // day = 1
    // month = 1
    // year = 1970

    // 1 year = 31,536,000 seconds
    // 1 month = 2,678,400 seconds
    // 1 day = 86,400 second

    int day1 = *(&day);
    int month1 = *(&month);
    int year1 = *(&year);

    if(second<86400){printf("January 1,1970"); printf("\n"); return;}

    // If the seconds are bigger than a year, add to the year1 variable
    while(second>=31536000){
        year1++;
        second = second - 31536000;
    }
    // If the seconds are bigger than a month but smaller than a year, add to the month1 variable
    while(second>=2678401){
        month1++;
        second = second - 2678401;
    }
    // If the seconds are bigger than a day but smaller than a month, add to the day1 variable
    while(second>=86400)
    {
        if(month1==1){
            if(day1>=31){
                day1 = 31;
            }
            else
            {
                day1++;
            }
        }
        else{
            if(day1>=30){
                day1 = 30;
            }
            else
            {
                day1++;
            }
        }
        second = second - 86400;
    }

    // Different print scenarios depending on the month
    // Includes leap year and non-leap year cases for February
    // Does not consider if the other months are either 30 or 31 days
    if(month1==1){printf("January %d,%d\n",day1,year1);}
    if(month1==2){
        if(day1>=29 && (year1%4==0 || year1%400==0)){day1=29;} // For leap years
        else if(day1>=28 && (year1%4!=0 || year1%100==0)){day1=28;} // For non-leap years
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