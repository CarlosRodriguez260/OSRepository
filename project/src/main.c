#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <header.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <FILE_NAME> <ROWS> <COLUMNS>\n", argv[0]);
        return 1;
    }

    const char *FILE_NAME = argv[1];
    int ROWS = atoi(argv[2]);
    int COLUMNS = atoi(argv[3]);
    size_t expected_floats = ROWS * COLUMNS;
    
    FILE *fptr = fopen(FILE_NAME, "rb");
    if (fptr == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // Verify file size
    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    rewind(fptr);
    
    if (file_size != expected_floats * sizeof(float)) {
        fprintf(stderr, "File size doesn't match expected dimensions\n");
        fclose(fptr);
        return 1;
    }

    float *numbers = malloc(expected_floats * sizeof(float));
    if (!numbers) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(fptr);
        return 1;
    }

    size_t read = fread(numbers, sizeof(float), expected_floats, fptr);
    if (read != expected_floats) {
        fprintf(stderr, "Failed to read all expected values\n");
        free(numbers);
        fclose(fptr);
        return 1;
    }

    // Print first 5 values as a sample
    int count = 0; 
    for (int i = 0; i < ROWS*COLUMNS && i < expected_floats; i++) {
        printf("%f\n", -(numbers[i]/5));
        count++;
    }
    printf("Obtained %d values, which is equal to %d\n", count, ROWS*COLUMNS);

    // Call CSV Creator Code
    csvCreator(numbers, ROWS, COLUMNS);

    free(numbers);
    fclose(fptr);
    return 0;
}