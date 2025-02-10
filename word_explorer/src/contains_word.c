#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


#define BUFFER_SIZE 4096

/**
 * 
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