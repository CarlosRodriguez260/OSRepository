#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


/**
 * 
 */

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