#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "printArray.h"

int main() {
  int type;
  printf("1 for char; 2 for integer; 3 for float \n");
  scanf("%d \n", &type);

  // Array in the printArray function is a variable, subject to change
  // in type depending on the var type. Strings are passed on as
  // arrays of chars already, but for the other two we have to create
  // the array.

  if (type == 1) {
    char input[100];
    fgets(input, sizeof(input), stdin);
    char* cPtr = &input;
    PrintArray(cPtr, type);
  }  // char
  else if (type == 2) {
    int arr[500];
    bool stop = false;
    int index = 0;
    while (!stop) {
      int value;
      scanf("%d", &value);

      if (value < 0) {
        arr[index] = value;
        break;
      }
      arr[index] = value;
      index++;
    }
    PrintArray(arr, type);
  }  // integer
  else if (type == 3) {
    float arr[500];
    bool stop = false;
    int index = 0;
    while (!stop) {
      float value;
      scanf("%f", &value);

      if (value <= 0.0000000 || isnan(value)) {
        arr[index] = value;
        break;
      }
      arr[index] = ceil(value * 1000.0) / 1000.0;
      index++;
    }
    PrintArray(arr, type);
  }  // float, rounded to 3 decimal spaces
  else {
    printf("Invalid Entry! \n");
  }
  return 0;
}