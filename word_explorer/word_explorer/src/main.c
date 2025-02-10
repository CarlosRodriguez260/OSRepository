#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

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

void ExploreDirectory(const char *dirpath, const char *word) {
  DIR *dir = opendir(dirpath);
  if (!dir) {
    perror("Error opening directory");
    return;
  }

  struct dirent *entry;
  char filepath[1024];

  while ((entry = readdir(dir)) != NULL) {
    // Ignore "." and ".." entries
    if (entry->d_name[0] == '.') continue;

    snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);

    struct stat path_stat;
    stat(filepath, &path_stat);

    if (S_ISREG(path_stat.st_mode)) {
      if (ContainsWord(filepath, word)) {
        printf("Found in: %s\n", filepath);
      }
    }
  }

  closedir(dir);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <folder_path> <word>\n", argv[0]);
    return 1;
  }

  ExploreDirectory(argv[1], argv[2]);

  return 0;
}
