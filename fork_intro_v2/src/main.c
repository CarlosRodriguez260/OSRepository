#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
  int originalPID = getpid();
  pid_t pid1;
  pid_t pid2;
  pid_t pid11;

  pid1 = fork();
  if(originalPID == getpid()){
    pid2 = fork();
  }
  // Check for first child
  if(pid1==0){
    printf("I am the first child. My PID should be +1 to my parent, and it is: %d \n",getpid());
    //execlp("firefox","firefox",NULL);
    int originalPID2 = getpid();
    pid11 = fork();

    if(pid11==0){
      printf("I am the child of child %d, and my PID is: %d \n", originalPID2, getpid());
      while(1){printf("A");}
    }
  }
  // Check for second child
  else if(pid2==0){
    printf("I am the second child. My PID should be +2 to my parent, and it is: %d \n",getpid());
  }
  // If neither, you are parent
  else{
    printf("I am the parent. My PID is: %d \n", getpid());
  }
}
