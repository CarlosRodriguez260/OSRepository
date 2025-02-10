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

int main(int argc, char *argArray[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <folder_path> <word>\n", argArray[0]);
    return 1;
  }

  ExploreDirectory(argArray[1], argArray[2]);

  return 0;
}
