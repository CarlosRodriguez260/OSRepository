#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief Recursively explores a directory and searches for a given word in
 * regular files.
 *
 * This function opens the directory specified by "dirpath" and iterates through
 * its entries. For each entry, it constructs the full file path and checks if
 * the entry is a readable file. If the entry is a readbale file, the function
 * calls "ContainsWord" to check if the specified "word" is present in the file.
 * If the word is found, the file path is printed to the console.
 *
 * @param dirpath The path to the directory to be explored.
 * @param word The word to search for within the files of the directory.
 *
 * @note This function ignores hidden files and directories (those starting with
 * '.').
 * @note The function does not recursively explore subdirectories; it only
 * processes files in the specified directory.
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