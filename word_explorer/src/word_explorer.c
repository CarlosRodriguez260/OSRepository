#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief The main entry point of the program.
 *
 * This program searches for a specified word in all regular files within a
 * given directory. It takes two command-line arguments: the path to the
 * directory and the word to search for. The program calls the
 * "ExploreDirectory" function to perform the search and prints the paths of
 * files where the word is found with "ContainsWord".
 *
 * @param argc The number of command-line arguments (Automatically given by
 * command-line or other program).
 * @param argArray An array of command-line arguments, where:
 *        - "argArray[0]" is the program name.
 *        - "argArray[1]" is the path to the directory to search.
 *        - "argArray[2]" is the word to search for in the files.
 *
 * @return int Returns 0 on successful execution, or 1 if the number of
 * arguments is incorrect.
 *
 * @note The program expects exactly two command-line arguments. If the number
 * of arguments is incorrect, it prints a usage message and exits with a
 * non-zero status.
 * 
 * @note This program must be built first in the "build" folder of this
 * project in order for the lab excercise to function correctly
 */

int main(int argc, char *argArray[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <folder_path> <word>\n", argArray[0]);
    return 1;
  }

  ExploreDirectory(argArray[1], argArray[2]);

  return 0;
}
