#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

typedef struct
{
  int x;
  int y;
} BallPosition;

#define ENTER_NCURSES 10
int mode = 0; // Mode 0 = Automatic. Mode 1 = Manual
int counter_val = 0;
int row;
int col;
void * keyboardInputs();
void * ballPlotter();
BallPosition ball_pos;

int main()
{
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

  pthread_create(&inputThread,&attr,keyboardInputs,NULL);
  pthread_create(&plotterThread,&attr,ballPlotter,NULL);

  while (1)
  {
    if(mode==0){
      ball_pos.x += dx;
      ball_pos.y += dy;
      if (ball_pos.x >= col - 1)
      {
        dx = -1;
      }
      if (ball_pos.y >= row - 1)
      {
        dy = -1;
      }
      if (ball_pos.x <= 1)
      {
        dx = 1;
      }
      if (ball_pos.y <= 1)
      {
        dy = 1;
      }
    }
    else if(mode==1){
      //
    }

    usleep(100000); /* Duerme por 100ms */
  }

  getch();
  endwin();

  return 0;
}

void * keyboardInputs(){
  nodelay(stdscr, TRUE);
  int key_stroke;
  bool break_loop = false;
  while(!break_loop)
  {
    key_stroke = getch();
    switch (key_stroke)
    {
    case KEY_UP:
      /* code */
      if(mode==1){
        if(ball_pos.y>=1){
          ball_pos.y--;
        }
      }
      break;
    case KEY_DOWN:
      /* code */
      if(mode==1){
        if(ball_pos.y<row-1){
          ball_pos.y++;
        }
      }
      break;
    case KEY_LEFT:
      /* code */
      if(mode==1){
        if(ball_pos.x>=1){
          ball_pos.x--;
        }
      }
      break;
    case KEY_RIGHT:
      /* code */
      if(mode==1){
        if(ball_pos.x<col-1){
          ball_pos.x++;
        }
      }
      break;
    case 32:
      //
      if(mode==0){
        mode = 1;
      }
      else{
        mode = 0;
      }
      break;
    case KEY_BACKSPACE:
      /* code */
      kill(getpid(),SIGTERM);
    }
    usleep(100000); // Small delay to avoid high CPU usage
  }
}

void * ballPlotter(){
  while(1){
    clear();
    getmaxyx(stdscr, row, col); /* Obtiene el numbero de filas y columnas */
    char mode_name[20];

    if(mode==0){
      sprintf(mode_name,"%s","Automatic");
    }
    else{
      sprintf(mode_name,"%s","Manual");
    }
    mvprintw(0, 0, "%d", counter_val++);
    mvprintw(1, 0, "%s", mode_name);
    mvprintw(ball_pos.y, ball_pos.x, "o");
    refresh();
    usleep(100000);
  }
}