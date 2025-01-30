#include "FillDate.h"
#include <time.h>

int main(){
    int year = 1970;
    int day = 1;
    int month = 1;
    int * yPtr = &year;
    int * dPtr = &day;
    int * mPtr = &month;

    int second1;
    int second2;
    int second3;

    printf("Please enter 3 inputs of seconds \n");
    scanf("%d %d %d", &second1, &second2, &second3);
    printf("Here are your dates:\n");
    FillDate(second1, dPtr, mPtr, yPtr);
    FillDate(second2, dPtr, mPtr, yPtr);
    FillDate(second3, dPtr, mPtr, yPtr);

    // NOTE: Test cases were checked with https://www.epochconverter.com/
    //       to ensure compliance with leap years and other constraints.
    //
    // FillDate(360000,day,month,year); // Prints "January 5, 1970"
    // FillDate(0,day,month,year); // Prints "January 1, 1970"
    // FillDate(2628000,day,month,year); // Prints "January 31, 1970"
    // FillDate(1987200,day,month,year); // Prints "January 24, 1970"
    // FillDate(5133600,day,month,year); // Prints "March 1,1970"
    // FillDate(36756000,day,month,year); // Prints "March 2, 1971"
    // FillDate(1702944000,day,month,year); // Prints "December 19, 2023"
    // FillDate(1708164000,day,month,year); // Prints "February 17, 2024"
    // FillDate(2678400,day,month,year); // Prints "January 31, 1970"
    // FillDate(946699200,day,month,year); // Prints "January 1, 2000"
    // FillDate(298313721,dPtr,mPtr,yPtr); // June 15, 1979
    return 0;
}