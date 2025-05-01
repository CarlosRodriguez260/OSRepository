#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <header.h>

// Function for creating the CSV
void csvCreator(float * numbers, int rows, int columns) {
  printf("Initiating CSV parsing\n");
  FILE *csvptr = NULL;

  if(csvptr==NULL){
    csvptr = fopen("./decoded_image.csv", "w");
  }

  // Algorithm for writing the value into the csv
  int index = 0;
  int i = 0;
  int j = 0;
  while(j<=columns-1){
    if(j==columns-1 && i==rows-1){
      // Final Value
      fprintf(csvptr, "%f", -(numbers[index]/5));
      break;
    }
    else if(j==columns-1 && i<rows-1){
      // Final Value of Row
      fprintf(csvptr, "%f\n", -(numbers[index]/5));
      i++;
      j = 0;
      continue;
    }
    else{
      // Write value into CSV
      fprintf(csvptr, "%f,", -(numbers[index]/5));
      j++;
    }
    index++;
  }
  
  printf("Parsing complete!\n");
  fclose(csvptr);
}