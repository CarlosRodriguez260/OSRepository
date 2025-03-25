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
void write_elapsed_time(double elapsed_time);
int x_coord = 0;
int y_coord = 0;
sem_t* sem1; // Updating coordinate values

/**
 * @brief A program where
 *
 * @details
 *
 * @return N/A
 */

// Function called each time the 50ms timer goes off
void timerHandler(union sigval sv) { sem_post(sem1); }

int main(){
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

  // Create timer variable and structs
  timer_t timer;
  struct sigevent sev;
  struct itimerspec its;

  // Setup the structs for handling timer going off
  sev.sigev_notify = SIGEV_THREAD;  // Notification method (thread creation)
  sev.sigev_notify_function = &timerHandler;
  sev.sigev_value.sival_ptr = NULL;  // Pass data to handler
  sev.sigev_notify_attributes = NULL;

  // Create the timer and link the sigevent struct
  timer_create(CLOCK_REALTIME, &sev, &timer);

  // Setup the structs for when the timer goes off
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 50000000; // Initial timer set off
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 50000000; // Concurrent timer set off

  // Start ncurses window
  initscr();
  noecho();
  curs_set(0);

  // Open the paths file
  FILE* file;
  char line[LINE_LENGTH];
  file = fopen(filename, "r");

  // Setup the timespec struct for clock_gettime
  struct timespec start_time, end_time;
  if (file == NULL) {
    perror("Error trying to open text file");
  }

  // Arm the timer
  timer_settime(timer, 0, &its, NULL);

  while (1) {
    //Start measuring time 
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &start_time);
    sem_wait(sem1);

    // Read through all the lines, so stop the program
    if(fgets(line, sizeof(line), file)==NULL){

      // Close the path file if its open
      if(file){
        fclose(file);
      }
      
      // In case the log file is still open, close it
      FILE *time_file = fopen("../logs/elapsed_times.log", "a");
      if (time_file) {
          fclose(time_file);
      }

      // Unlink semaphore 1
      sem_unlink(SEMNAME_1);

      // Kill the program
      kill(getpid(),SIGINT);
    }


    line[strcspn(line, "\n")] = '\0';
    sscanf(line, "%d,%d", &x_coord, &y_coord);
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &end_time);
    // Stop measuring time

    // Calculate the time (in milliseconds) that happened during the update
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
    write_elapsed_time(elapsed_time);

    // Draw the asterisk position with ncurses
    erase();
    mvprintw(y_coord,x_coord,"*");
    refresh();
  }
  return 0;
}