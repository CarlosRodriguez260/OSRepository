#include <curses.h> /* ncurses.h incluye stdio.h */
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Global variables
#define LINE_LENGTH 1024
#define SEMNAME_1 "/sem1"
char filename[8192];
void * fileReader();
int x_coord = 0;
int y_coord = 0;
int col;
int row;
sem_t* sem1;


/**
 * @brief A program where a ball bounces against obstacles and destroys them.
 *
 * @details Utilizing ncurses, we have built a program that generates a stage
 * filled with obstacles, which are destroyed by a moving ball. There are 3
 * levels of difficulty, each filling up more and more of the stage with
 * obstacles. Level 1, or low, only makes obstacles cover 25% of the stage.
 * Level 2, or medium, makes the obstacles cover 50% of the stage. Level 3, or
 * high, makes the obstacles cover 75% of the screen. Starting from the same
 * position, the ball will move around and its objective is to destroy all the
 * objectives, and once that is done, the program closes automatically. Threads
 * are utilized to handle the plotting of the ball and stage, as well as the
 * collisions and movement of the ball.
 *
 * @return N/A
 */

void timerHandler(union sigval sv){
  sem_post(sem1);
}

int main() {
  sem1 = sem_open(SEMNAME_1, O_CREAT, 0666, 1);

  sprintf(filename,"%s","../paths/path_2.txt");
  timer_t timer;
  struct sigevent sev;
  struct itimerspec its;
  
  sev.sigev_notify = SIGEV_THREAD;  // Notification method (thread creation)
  sev.sigev_notify_function = &timerHandler;
  sev.sigev_value.sival_ptr = NULL;  // Pass data to handler
  sev.sigev_notify_attributes = NULL;

  // Create the timer
  timer_create(CLOCK_REALTIME, &sev, &timer);

  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 50000000;
  its.it_interval.tv_sec = 0; 
  its.it_interval.tv_nsec = 50000000;

  // Launch the file reader on a thread
  pthread_t readerThread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_create(&readerThread, &attr, fileReader, NULL);

  // Arm the timer
  timer_settime(timer,0,&its,NULL);

  // initscr();
  // noecho();
  // curs_set(0);

  while(1){
    sleep(1);
    // clear();
    // getmaxyx(stdscr, row, col);
  }

  getch();
  endwin();

  return 0;
}

void * fileReader(){
  FILE *file;
  char line[LINE_LENGTH];

  file = fopen(filename, "r");
  if (file==NULL){
    perror("Error trying to open text file");
    return 1;
  }

  while(fgets(line, sizeof(line), file) != NULL){
    // Remove trailing newline if present
    line[strcspn(line, "\n")] = '\0';

    sem_wait(sem1);
    sscanf(line, "%d,%d", &x_coord, &y_coord);
    printf("Line: %d\n", line);
  }

  sem_unlink(SEMNAME_1);
  kill(getpid(),SIGINT);
}