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
 * This program prompts the user to input a directory path and a word to search
 * for. It then forks a child process to execute the "word_explorer" program,
 * which is expected to be located in the "../../word_explorer/build/"
 * directory. The child process runs the "word_explorer" program with the
 * provided directory and word as arguments. The parent process waits for the
 * child process to complete before printing a confirmation message.
 *
 * @note The "word_explorer" program must be built and located in the specified
 * directory
 *       ("../../word_explorer/build/") for this program to work correctly.
 *
 * @warning If the "word_explorer" program is not found or cannot be executed,
 * the child process will terminate without any indication of failure to the
 * parent process.
 */

int main() {
  pid_t pid;
  char directory[50];

  // For this program layout, we look at "../test_files"
  printf("Type in the directory where you want to look: \n");
  scanf("%s", directory);

  char word[50];
  printf("Type the word you want to find in said directory: \n");
  scanf("%s", word);

  // WE HAVE TO BUILD THE "word_explorer" PROGRAM FIRST IN THE
  // "word_explorer/build" FOLDER FOR THIS TO WORK
  char *argArray[] = {"../../word_explorer/build/word_explorer", directory,
                      word, NULL};

  // Create the child process that will execute "word_explorer"
  pid = fork();
  if (pid == 0) {
    printf("I am the child, and I will look for the word %s!\n", word);
    execv(argArray[0], argArray);
    exit(0);
  } else {
    // Make parent wait for child to be finished
    wait(NULL);
    printf("I am the parent, and I have waited on my child.\n");
  }
}