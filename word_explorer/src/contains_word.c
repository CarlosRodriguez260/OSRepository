#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
/**
 * @brief Checks if a given word is present in the specified file.
 *
 * This function opens the file located at "filepath" and reads its contents in
 * chunks of size "BUFFER_SIZE". It then searches for the presence of the "word"
 * in each chunk. If the word is found, the function returns 1; otherwise, it
 * returns 0.
 *
 * @param filepath The path to the file to be searched.
 * @param word The word to search for within the file.
 * @return Returns 1 if the word is found in the file, 0 if the word is not
 * found or if an error occurs while opening or reading the file.
 *
 * @note The function uses a buffer of size "BUFFER_SIZE" to read the file in
 * chunks. If the file is larger than the buffer, the function will continue
 * reading until the entire file is processed or the word is found.
 */

int ContainsWord(const char *filepath, const char *word) {
  int fd = open(filepath, O_RDONLY);
  if (fd < 0) {
    perror("Error opening file");
    return 0;
  }

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[bytes_read] = '\0';
    if (strstr(buffer, word) != NULL) {
      close(fd);
      return 1;
    }
  }

  close(fd);
  return 0;
}