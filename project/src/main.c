#define _GNU_SOURCE
#include <header.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Main function to read binary float data and convert to CSV.
 *
 * @param argc Number of command-line arguments.
 * @param argv Arguments: [program_name, FILE_NAME, ROWS, COLUMNS].
 * @return int 0 on success, 1 on error.
 */

int main(int argc, char *argv[]) {
  // Check for correct command-line arguments
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <FILE_NAME> <ROWS> <COLUMNS>\n", argv[0]);
    return 1;
  }

  const char *FILE_NAME = argv[1];          // Input binary file path
  int ROWS = atoi(argv[2]);                 // Expected rows in the data
  int COLUMNS = atoi(argv[3]);              // Expected columns in the data
  size_t expected_floats = ROWS * COLUMNS;  // Total expected float values

  // Open the binary file in read mode
  FILE *fptr = fopen(FILE_NAME, "rb");
  if (fptr == NULL) {
    perror("Failed to open file");
    return 1;
  }

  // Verify file size matches expected dimensions
  fseek(fptr, 0, SEEK_END);      // Move to end of file
  long file_size = ftell(fptr);  // Get file size in bytes
  rewind(fptr);                  // Reset file pointer to start

  if (file_size != expected_floats * sizeof(float)) {
    fprintf(stderr, "File size doesn't match expected dimensions\n");
    fclose(fptr);
    return 1;
  }

  // Allocate memory for the float data
  float *numbers = malloc(expected_floats * sizeof(float));
  if (!numbers) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(fptr);
    return 1;
  }

  // Read binary data into the array
  size_t read = fread(numbers, sizeof(float), expected_floats, fptr);
  if (read != expected_floats) {
    fprintf(stderr, "Failed to read all expected values\n");
    free(numbers);
    fclose(fptr);
    return 1;
  }

  // Print first 5 values as a sample (after dividing by 5 and negating)
  int count = 0;
  for (int i = 0; i < ROWS * COLUMNS && i < expected_floats; i++) {
    printf("%f\n", -(numbers[i] / 5));
    count++;
  }
  printf("Obtained %d values, which is equal to %d\n", count, ROWS * COLUMNS);

  // Generate CSV file from the data
  csvCreator(numbers, ROWS, COLUMNS);

  // Cleanup
  free(numbers);
  fclose(fptr);
  return 0;
}