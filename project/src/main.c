#include "FillDate.h"

int main(){
    int* year = 1970;
    int* day = 1;
    int* month = 1;
    int second1;
    int second2;
    int second3;

    printf("");
    scanf("%d %d %d \n", &second1, &second2, &second3);
    FillDate(second1, day, month, year);
    FillDate(second2, day, month, year);
    FillDate(second3, day, month, year);

    // NOTE: Test cases were checked with https://www.epochconverter.com/
    //       to ensure compliance with leap years and other constraints.
    // FillDate(360000,day,month,year); // Prints "January 5, 1970"
    // FillDate(0,day,month,year); // Prints "January 1, 1970"
    // FillDate(2628000,day,month,year); // Prints "January 31, 1970"
    // FillDate(1987200,day,month,year); // Prints "January 24, 1970"
    // FillDate(5133600,day,month,year); // Prints "February 28,1970"
    // FillDate(36756000,day,month,year); // Prints "February 28, 1971"
    // FillDate(1702944000,day,month,year); // Prints "December 18, 2023"
    // FillDate(1708164000,day,month,year); // Prints "February 17, 2024"
    // FillDate(2678400,day,month,year); // Prints "January 31, 1970"
    // FillDate(946699200,day,month,year); // Prints "January 1, 2000"
    FillDate(298313721,day,month,year); // Prints "February 29,2024"
    return 0;
}