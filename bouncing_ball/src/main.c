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
int dx, dy;
int difficulty;
dx = 1;
dy = 1;
void * ballMover();
void * plotter();
void * collisioner();
BallPosition ball_pos;
int matrix[15][25];

int main()
{
  printf("Select a difficulty:\n1. Low\n2. Medium\n3. High\n");
  scanf("%d",&difficulty);

  int max_twos = (299 * (25 * difficulty)) / 100;
  for(int i = 0; i<15; i++){
    for(int j = 0; j<25; j++){
      if(i==0 || i==14 || j == 0 || j == 24){
        matrix[i][j]=1;
        continue;
      }

      int random = rand();
      if(random%2==0 && random%3==0 && max_twos>=1 && i!=1 && j!=1){
        matrix[i][j]=2;
        max_twos--;
      }
      else{
        matrix[i][j]=0;
      }

    }
  }

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  ball_pos.x = 1;
  ball_pos.y = 1;

  pthread_t moverThread, plotterThread,collisionThread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_create(&plotterThread,&attr,plotter,NULL);
  pthread_create(&moverThread,&attr,ballMover,NULL);
  pthread_create(&collisionThread,&attr,collisioner,NULL);

  getch();
  endwin();

  return 0;
}

void * plotter(){
  while(1){
    clear();
    for(int i = 0; i<15; i++){
      for(int j = 0; j<25; j++){
        if(matrix[i][j]==0){
          mvprintw(i,j," ");
        }
        else if(matrix[i][j]==1){
          mvprintw(i,j,"*");
        }
        else if(matrix[i][j]==2){
          mvprintw(i,j,"#");
        }
      }
    }
    mvprintw(ball_pos.y,ball_pos.x,"o");
    refresh();
    usleep(100000);
  }
}

void * ballMover(){
  while (1)
  {
    if(mode==0){
      ball_pos.x += dx;
      ball_pos.y += dy;
    }
    usleep(100000); /* Duerme por 100ms */
  }
}

void * collisioner(){
  while (1)
  {
    int future_x = ball_pos.x += dx;
    int future_y = ball_pos.y += dy;

    // Check for diagonal collisions
    // Check for non-diagonal collisions
    usleep(100000); /* Duerme por 100ms */
  }
}