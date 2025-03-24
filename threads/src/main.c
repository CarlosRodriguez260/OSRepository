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
typedef struct {
  int x;
  int y;
} BallPosition;

#define ENTER_NCURSES 10
#define SEMNAME_1 \
  "/sem1"  // Used for preventing race conditions between plotting and collision
           // calculations
int mode = 0;
int counter_val = 0;
int row;
int col;
int dx, dy;
int difficulty;
dx = 1;
dy = 1;
void* ballMover();    // Function used in thread for moving the ball
void* plotter();      // Function used in thread to plot the ball and the stage
void* collisioner();  // Function used in thread to calculate collisions
BallPosition ball_pos;
int matrix[15][25];
int stuck_counter = 0;  // Used to prevent infinite loops
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

int main() {
  timerUpdate(); 
}