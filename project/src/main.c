 #define _GNU_SOURCE
 #include <ctype.h>
 #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/mman.h>
 #include <sys/stat.h>
 #include <unistd.h>
 #include <string.h>
 
 /**
  * @brief Main function to process the CSV and compute row sums.
  * 
  * The function maps the CSV file into memory, processes the characters, and sums up the numeric values
  * in each row separated by commas. Once the row sum is computed, the program prints the sum for that row.
  * The program stops when it reaches the end of the CSV.
  * 
  * @return N/A
  */

  // Gloval Variables
  #define N_NUMBERS 200000
  char FILE_NAME[8192];
  int ROWS;
  int COLUMNS;

 int main(int argc, char * argArray[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <FILE_NAME> <#ROWS> <#COLUMNS>\n", argArray[0]);
        return 1;
    }

    FILE *fptr;
    float numbers[N_NUMBERS];
    sprintf(FILE_NAME, "%s", argArray[1]); 
    ROWS = atoi(argArray[2]);
    COLUMNS = atoi(argArray[3]);

     // Open the CSV file for reading and writing
     fptr = fopen(FILE_NAME, "rb");

        // checking if the file is opened successfully
    if (fptr == NULL) {
        printf(
            "The file is not opened. The program will "
            "now exit.");
        exit(0);
    }

    fread(&numbers, sizeof(float), N_NUMBERS, fptr);

    for (int i = 0; i < 30; i++) {
        printf("%f\n", numbers[i]);
    }
     // Initialize variables for processing the file
     int index = 0; // Index to traverse the file
 
     return 0; // Successful execution
 }