#include "FillDate.h"
#include <stdio.h>

/**
 * @brief Converts a given number of seconds since January 1, 1970 
 *        into a human-readable date (Month, Day, Year).
 *
 * This function calculates the equivalent date (Month, Day, Year) 
 * for a given number of seconds since January 1, 1970.
 * 
 * @param second The number of seconds since January 1, 1970.
 * @param day Pointer to an integer that stores the initial value of the first day. Value is always equal to 1.
 * @param month Pointer to an integer that stores the initial value representing the first month, January. Value is always equal to 1.
 * @param year Pointer to an integer that stores the computed year.
 * @return The appropriate date after adding the seconds in format of (Month Day, Year) 
 *  - (NOTE: Not an actual return, the date is printed).
*/

void FillDate(long second, int* day, int* month, int* year){
    // Initial values
    int day1 = *day;
    int month1 = *month;
    int year1 = *year;

    // Array to store the number of days in each month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // If we don't exceed a day worth of seconds, return the original date
    if(second < 86400){
        printf("January 1, 1970\n");
        return;
    }

    // Calculate the number of years
    while(second >= 31536000){
        // Check if the current year is a leap year
        if((year1 % 4 == 0 && year1 % 100 != 0) || year1 % 400 == 0){
            if(second >= 31622400){ // Leap year has 366 days
                second -= 31622400;
                year1++;
            } else {
                break;
            }
        } else {
            second -= 31536000; // Non-leap year has 365 days
            year1++;
        }
    }

    // Calculate the number of months
    while(second >= 86400){
        // Check if the current year is a leap year to adjust February days
        if((year1 % 4 == 0 && year1 % 100 != 0) || year1 % 400 == 0){
            daysInMonth[1] = 29;
        } else {
            daysInMonth[1] = 28;
        }

        // Calculate the number of days in the current month
        int daysInCurrentMonth = daysInMonth[month1 - 1];

        if(second >= daysInCurrentMonth * 86400){
            second -= daysInCurrentMonth * 86400;
            month1++;
            if(month1 > 12){
                month1 = 1;
                year1++;
            }
        } else {
            break;
        }
    }

    // Calculate the number of days
    day1 += second / 86400;
    second %= 86400;

    // Adjust the day if it exceeds the number of days in the current month
    if((year1 % 4 == 0 && year1 % 100 != 0) || year1 % 400 == 0){
        daysInMonth[1] = 29;
    } else {
        daysInMonth[1] = 28;
    }

    if(day1 > daysInMonth[month1 - 1]){
        day1 -= daysInMonth[month1 - 1];
        month1++;
        if(month1 > 12){
            month1 = 1;
            year1++;
        }
    }

    // Different print scenarios depending on the month
    const char* months[] = {"January", "February", "March", "April", "May", "June", 
                            "July", "August", "September", "October", "November", "December"};
    printf("%s %d, %d\n", months[month1 - 1], day1, year1);
}