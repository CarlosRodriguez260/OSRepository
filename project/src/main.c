#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "header.h"

int main() {
  srand(time(NULL));
  char input[100];
  char output[100];

  printf("Enter a string: ");
  fgets(input, sizeof(input), stdin);
  UpperRand(input, output);
  PrintArray(input, output);
  return 0;
}