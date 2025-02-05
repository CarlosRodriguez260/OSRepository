#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
  int entry;
  printf("Enter any number: ");
  scanf("%d", &entry);

  int parent_pid = getpid();
  pid_t child1 = -1;
  pid_t child2 = -1;
  pid_t child3 = -1;

  // Create the first child
  child1 = fork();

  // Create the second child
  if(getpid()==parent_pid){
    child2 = fork();
  }

  // Create the third child
  if(getpid()==parent_pid){
    child3 = fork();
  }
  //All childs come from the first parent


  // We are in first child
  if(child1==0){
    int count = 10;
    while(count>=0){
      printf("[%d] child one: %d \n", getpid(), entry);
      entry++;
      count--;
      sleep(1);
    }
    exit(0);
  }
  // We are in second child
  else if(child2==0){
    int count = 7;
    while(count>=0){
      printf("[%d] child two: %d \n", getpid(), entry);
      entry--;
      count--;
      sleep(2);
    }
    exit(0);
  }
  // We are in third child
  else if(child3==0){
    sleep(15);
    printf("[%d] child three: \n", getpid());
    execlp("ifconfig",NULL);
    exit(0);
  }
  // We are in parent
  else {
    wait(NULL);
    wait(NULL);
    wait(NULL);
    printf("Parent: My work here is done \n");
  }

  return 0;
}