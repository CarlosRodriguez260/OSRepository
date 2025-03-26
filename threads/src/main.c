#include <curses.h> /* ncurses.h incluye stdio.h */
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "header.h"

// Global variables
#define LINE_LENGTH 1024
#define SEMNAME_1 "/sem1"
char filename[8192];
void* fileReader();
void write_elapsed_time(double elapsed_time);
int x_coord = 0;
int y_coord = 0;
sem_t* sem1; // Refreshing the screen

/**
 * @brief Program that displays a moving asterisk along a predefined path.
 * 
 * @details This program performs the following operations:
 * 1. Initializes by removing any existing elapsed time log file
 * 2. Creates and initializes a semaphore for coordinate synchronization
 * 3. Prompts the user to select which path file to use (path 1 or 2)
 * 4. Launches a thread which is in charge of reading the file and updating the 
 *    coordinates of the asterisk. The thread uses usleep(50000) instead of a 
 *    POSIX timer.
 * 5. Initializes the ncurses environment for display
 * 6. Opens and reads the selected path file containing coordinate data
 * 7. Enters two loops:
 *    - The loop in main is used for refreshing the ncurses screen. A semaphore is used
 *      to update the screen once the coordinates are updated in the thread
 *    - The loop in the thread which reads every line from the file and updates the coordinates
 * 
 * The program finishes once:
 * - All coordinates from the file have been processed
 * 
 * @note The source code that writes the data into the log file is found in writer.c 
 * @warning The program assumes the existence of specific directory structures for logs and paths.
 * 
 * @return N/A
 */

int main() {
  // Delete the old elapsed_times.log to get a fresh one, if it already exists
  if (access("../logs/elapsed_times.log", F_OK) == 0) {  
    remove("../logs/elapsed_times.log");  // Delete the file if it exists
  }
  
  // Open semaphore 1
  sem1 = sem_open(SEMNAME_1, O_CREAT, 0644, 0);

  // Let user choose which path file to look at
  int input;
  printf("Take path 1 or path 2?\n");
  scanf("%d",&input);

  if(input==1){
    sprintf(filename, "%s", "../paths/path_1.txt");
  }
  else{
    sprintf(filename, "%s", "../paths/path_2.txt");
  }

  // Launch the file reader on a thread
  pthread_t readerThread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_create(&readerThread, &attr, fileReader, NULL);

  // Start ncurses window
  initscr();
  noecho();
  curs_set(0);

  // Draw the asterisk position with ncurses
  // Semaphore 1 makes sure it happens right after the coordinates are updated on the thread
  while (1) {
    sem_wait(sem1);
    erase();
    mvprintw(y_coord, x_coord, "*");
    refresh();
  }
  return 0;
}

// Function version of the file reader code, which is called on a thread
void * fileReader() {
  FILE* file;
  char line[LINE_LENGTH];

  // Open the paths file
  file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error trying to open text file");
  }

  // Setup the timespec structs for clock_gettime
  struct timespec start_time, end_time;
  while(1){
    // Start measuring time
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &start_time);
    usleep(50000);

    // Break loop if line being read is NULL
    if(fgets(line, sizeof(line), file)==NULL){
      break;
    }

    line[strcspn(line, "\n")] = '\0';
    sscanf(line, "%d,%d", &x_coord, &y_coord);
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &end_time);
    //Stop measuring time

    // Post the semaphore to draw updated coordinates
    sem_post(sem1);

    // Calculate the time (in milliseconds) that happened during the update
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
    write_elapsed_time(elapsed_time);
  }

  // Close the paths file
  if(file){
    fclose(file);
  }

  // Unlink semaphore 1 and close the log file it's somehow still open
  sem_unlink(SEMNAME_1);
  FILE *time_file = fopen("../logs/elapsed_times.log", "a");
  if (time_file) {
      fclose(time_file);
  }

  // Kill the program
  kill(getpid(), SIGINT);
}