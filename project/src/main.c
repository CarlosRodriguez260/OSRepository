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
 int main() {
     // Open the CSV file for reading and writing
     int csv = open("./sum_data.csv", O_RDWR, 0600);
     if (csv == -1) {
         perror("Failed to open CSV file");
         return 1;
     }
 
     // Get file size using fstat
     struct stat sb;
     fstat(csv, &sb);
 
     // Map the CSV file into memory
     char *csv_chars = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, csv, 0);
     if (csv_chars == MAP_FAILED) {
         perror("Failed to map file into memory");
         return 1;
     }
 
     // Initialize variables for processing the file
     int index = 0; // Index to traverse the file
     char all_nums[200]; // String to hold the sum of the row
     float row_sum = 0; // Accumulator for the sum of the current row
     char num[200]; // Temporary string to hold a number
     int num_index = 0; // Index for the num array (stores the current number)
     int row = 1; // Row number counter
 
     // Loop through the CSV file until we reach the end
     while (1) {
         char character = csv_chars[index]; // Current character from the file
 
         // If a comma is found, process the current number and add it to the row sum
         if (character == ',') {
             row_sum = row_sum + atof(num); // Convert num to float and add to row_sum
             num[0] = '\0'; // Clear num string
             num_index = 0; // Reset num_index
         }
         // If a newline character is found, finish processing the row
         else if (character == '\n') {
              row_sum = row_sum + atof(num); // Add the last number in the row
              sprintf(all_nums, "%.2f", row_sum); // Format the row sum to 2 decimal places
              printf("[Row %d]: %s\n", row, all_nums); // Print the row sum
              row++; // Increment row number
              row_sum = 0; // Reset the row sum
              num[0] = '\0'; // Clear num array
              num_index = 0; // Reset num_index 
         }
         // If we reach the last character of the file, process the last row
         else if (index == sb.st_size - 1) {
             row_sum = row_sum + atof(num); // Add the last number in the row
             sprintf(all_nums, "%.2f", row_sum); // Format the row sum
             printf("[Row %d]: %s\n", row, all_nums); // Print the last row sum
             row++; // Increment row number
             row_sum = 0; // Reset the row sum
             num[0] = '\0'; // Clear num array
             num_index = 0; // Reset num_index
             printf("Done reading!\n"); // Notify that the processing is complete
             break; // Exit the loop
         }
         // If character is not a delimiter, it's part of a number
         else {
             num[num_index++] = character; // Append character to num array
         }
         index++; // Move to the next character
         usleep(10000); // Sleep for 10 milliseconds (for demonstration purposes)
     }
 
     // Clean up and close the file descriptor
     munmap(csv_chars, sb.st_size); // Unmap the file from memory
     close(csv); // Close the CSV file
 
     return 0; // Successful execution
 }