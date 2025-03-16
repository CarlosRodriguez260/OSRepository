#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

// Global Variables
typedef struct {
  int x;
  int y;
} BallPosition;

#define ENTER_NCURSES 10
int mode = 0;  // Mode 0 = Automatic. Mode 1 = Manual
int counter_val = 0;
int row;
int col;
void* keyboardInputs();  // Function used in thread to read user inputs
void* ballPlotter();     // Function used in thread to plot ball movement
BallPosition ball_pos;

/**
 * @brief Program that launches a ball around a stage, which bounces around.
 *
 * @details Utilizing ncurses, we made a program which launches a ball around a
 * set stage. The ball can move around in two modes: automatic and manual. In
 * automatic mode, the ball moves diagonally throughout the stage, colliding
 * with its borders. In manual mode, the user can use the arrow keys to move the
 * ball around to where they please. To switch between these modes, the spacebar
 * can be pressed. A thread is made to plot the movement of the ball, and
 * another one used to capture the inputs of the user.
 */
int main() {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  ball_pos.x = 10;
  ball_pos.y = 20;
  int dx, dy;
  dx = dy = 1;

  pthread_t inputThread, plotterThread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  // Start the threads
  pthread_create(&inputThread, &attr, keyboardInputs, NULL);
  pthread_create(&plotterThread, &attr, ballPlotter, NULL);

  // While loop in main checks for the collisions and
  // what to do in each case.
  while (1) {
    if (mode == 0) {
      ball_pos.x += dx;
      ball_pos.y += dy;
      if (ball_pos.x >= col - 1) {
        dx = -1;
      }
      if (ball_pos.y >= row - 1) {
        dy = -1;
      }
      if (ball_pos.x <= 1) {
        dx = 1;
      }
      if (ball_pos.y <= 1) {
        dy = 1;
      }
    } else if (mode == 1) {
      //
    }

    usleep(100000); /* Duerme por 100ms */
  }

  getch();
  endwin();

  return 0;
}

void* keyboardInputs() {
  nodelay(stdscr, TRUE);
  int key_stroke;
  bool break_loop = false;
  while (!break_loop) {
    key_stroke = getch();
    switch (key_stroke) {
      case KEY_UP:
        /*Make the ball move up */
        if (mode == 1) {
          if (ball_pos.y >= 1) {
            ball_pos.y--;
          }
        }
        break;
      case KEY_DOWN:
        /* Make the ball move down */
        if (mode == 1) {
          if (ball_pos.y < row - 1) {
            ball_pos.y++;
          }
        }
        break;
      case KEY_LEFT:
        /* Make the ball move left */
        if (mode == 1) {
          if (ball_pos.x >= 1) {
            ball_pos.x--;
          }
        }
        break;
      case KEY_RIGHT:
        /* Make the ball move right */
        if (mode == 1) {
          if (ball_pos.x < col - 1) {
            ball_pos.x++;
          }
        }
        break;
      case 32:
        // Spacebar press. Switch Modes
        if (mode == 0) {
          mode = 1;
        } else {
          mode = 0;
        }
        break;
      case KEY_BACKSPACE:
        /* Backspace kills the program */
        kill(getpid(), SIGTERM);
    }
    usleep(100000);  // Small delay to avoid high CPU usage
  }
}

void* ballPlotter() {
  while (1) {
    clear();
    getmaxyx(stdscr, row, col); /* Obtiene el numbero de filas y columnas */
    char mode_name[20];

    if (mode == 0) {
      sprintf(mode_name, "%s", "Automatic");
    } else {
      sprintf(mode_name, "%s", "Manual");
    }
    mvprintw(0, 0, "%d", counter_val++);
    mvprintw(1, 0, "%s", mode_name);
    mvprintw(ball_pos.y, ball_pos.x, "o");
    refresh();
    usleep(100000);
  }
}