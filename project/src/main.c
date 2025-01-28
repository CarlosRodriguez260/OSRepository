#include "FillDate.h"

int main(){
    FillDate(345667,1,1,1970); // Prints "January 5, 1970"
    FillDate(0,1,1,1); // Prints "January 1, 1970"
    FillDate(31536000,1,1,1970); // Prints "January 1, 1971"
    FillDate(2628000,1,1,1970); // Prints "February 1, 1970"
    FillDate(1987200,1,1,1970); // Prints "January 24, 1970"
    FillDate(5133600,1,1,1970); // Prints "February 28,1970"
    FillDate(36756000,1,1,1974); // Prints "February 28, 1971"
    FillDate(1702944000,1,1,1970); // Prints "January 1, 2024"
    FillDate(1708164000,1,1,1970); // Prints "February 29, 2024"
    return 0;
}