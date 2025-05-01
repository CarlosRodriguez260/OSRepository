#define _GNU_SOURCE
#include <header.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Converts a 2D float array into a CSV file with custom formatting.
 *
 * @param numbers Array of floats (row-major order: row0_col0, row0_col1, ...).
 * @param rows Total rows in the data.
 * @param columns Total columns per row.
 *
 * @attention In order to check the created CSV on the python program, you need
 * the location of the CSV file. With the current code, the path to the CSV from
 * the directory of the python program is "../project/build/decoded_image.csv".
 * @attention The python program is found outside the root of the project
 * folder.
 */

void csvCreator(float *numbers, int rows, int columns) {
  printf("Initiating CSV creation\n");

  FILE *csvptr = NULL;  // File pointer for the CSV

  // Open the CSV file in write mode (overwrites existing)
  if (csvptr == NULL) {
    csvptr = fopen("./decoded_image.csv", "w");
    if (csvptr == NULL) {
      perror("Failed to create CSV file");
      return;  // Exit if file creation fails
    }
  }

  // Track current position in the 2D array
  int index = 0;  // Linear index for the flat array
  int i = 0;      // Current row
  int j = 0;      // Current column

  // Loop through all elements
  while (j <= columns) {
    printf("Row %d | Column %d\n", i, j);  // Debug print

    // Case 1: Last element of the last row
    if (j == columns && i == rows) {
      fprintf(csvptr, "%f",
              -(numbers[index] / 5));  // No trailing comma/newline
      break;                           // Exit loop
    }
    // Case 2: Last column of a non-final row
    else if (j == columns && i < rows) {
      fprintf(csvptr, "%f\n", -(numbers[index] / 5));  // Newline after value
      i++;                                             // Move to next row
      j = 0;                                           // Reset column counter
      continue;  // Skip index++ (handled in else)
    }
    // Case 3: Non-terminal element
    else {
      fprintf(csvptr, "%f,", -(numbers[index] / 5));  // Comma-separated
      j++;                                            // Next column
    }
    index++;  // Always increment linear index
  }

  printf("CSV creation complete!\n");
  fclose(csvptr);  // Ensure file is closed
}