#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct
{
  int x;
  int y;
} BallPosition;

#define ENTER_NCURSES 10
#define SEMNAME_1 "/sem1"
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
int stuck_counter = 0;
sem_t * sem1;

int main()
{
  sem_unlink(SEMNAME_1);
  sem1 = sem_open(SEMNAME_1, O_CREAT, 0666, 1);

  printf("Select a difficulty:\n1. Low\n2. Medium\n3. High\n");
  scanf("%d",&difficulty);

  for(int i = 0; i<15; i++){
    for(int j = 0; j<25; j++){
      if(i==0 || i==14 || j == 0 || j == 24){
        matrix[i][j]=1;
      }
      else{
        matrix[i][j]=0;
      }
    }
  }

  int max_twos = (299 * (25 * difficulty)) / 100;
  while(max_twos>0){
    int i = rand() % (13 - 1 + 1) + 1;  // Row between 1 and 13
    int j = rand() % (23 - 1 + 1) + 1;  // Column between 1 and 23

    if((i==1 && j==1) || (i==2 && j==1) || (i==1 && j==2) || (i==2 && j==2) || matrix[i][j]==2){
      continue;
    }
    else{
      matrix[i][j]=2;
    }
    max_twos--;
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
  pthread_create(&collisionThread,&attr,collisioner,NULL);
  pthread_create(&moverThread,&attr,ballMover,NULL);

  getch();
  endwin();

  return 0;
}

void * plotter(){
  while(1){
    clear();
    bool found_two = false;
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
          found_two = true;
        }
      }
    }

    if(!found_two){
      clear();
      refresh();
      kill(getpid(),SIGKILL);
    }

    mvprintw(ball_pos.y,ball_pos.x,"o");
    char stuck_counter_reader[20];
    sprintf(stuck_counter_reader,"%d",stuck_counter);
    mvprintw(0,27,stuck_counter_reader);
    refresh();
    usleep(100000);
  }
}

void * ballMover(){
  while (1)
  {
    if(mode==0){
      sem_wait(sem1);
      ball_pos.x += dx;
      ball_pos.y += dy;
    }
    usleep(100000); 
  }
}

void * collisioner() {
  while (1) {
    stuck_counter++;
    int future_x = ball_pos.x + dx;
    int future_y = ball_pos.y + dy;

     // Prevent out of bounds movement
     if (future_x <= 0 || future_x >= 24) {  // Left or right wall
      dx = -dx;
      future_x = ball_pos.x;  // Stay in place
    }
    if (future_y <= 0 || future_y >= 14) {  // Top or bottom wall
      dy = -dy;
      future_y = ball_pos.y;  // Stay in place
    }

    // Collision checks
    
    // 1. Check for horizontal collision (left or right)
    if (matrix[ball_pos.y][future_x] == 1) {
      dx = -dx;
    } else if (matrix[ball_pos.y][future_x] == 2) {
      dx = -dx;
      matrix[ball_pos.y][future_x] = 0;
      stuck_counter = 0;
    }
    
    // 2. Check for vertical collision (above or below)
    if (matrix[future_y][ball_pos.x] == 1) {
      dy = -dy;
    } else if (matrix[future_y][ball_pos.x] == 2) {
      dy = -dy;
      matrix[future_y][ball_pos.x] = 0;
      stuck_counter = 0;
    }
    
    // 3. Check for direct diagonal collision
    else if (matrix[future_y][future_x] == 1) { 
      dx = -dx;
      dy = -dy;
    } else if (matrix[future_y][future_x] == 2) {  
      dx = -dx;
      dy = -dy;
      matrix[future_y][future_x] = 0; 
      stuck_counter = 0;
    }

    if(stuck_counter>=7500){
      dy = -dy;
      stuck_counter = 0;
    }
    sem_post(sem1);
    usleep(1000);  // Calculate collisions fast
  }
}
