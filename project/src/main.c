#define _GNU_SOURCE
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>

// Global Variables
cpu_set_t mask;
pthread_mutex_t mutex;
pthread_attr_t attr;
pthread_t th1;
pthread_t th2;
pthread_t th3;
void * thread1();
void * thread2();
void * thread3();
int * block;
bool stop = false;
int number_of_elements;
int nice1;
int nice2;

int main(int argc, char* arg[]) {
  // Make sure all parameters were passed
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <NE> <NICE_1> <NICE_2>\n",
            arg[0]);
    return 1;
  }

  srand(time(NULL));
  CPU_ZERO(&mask);
  pthread_attr_init(&attr);
  pthread_mutex_init(&mutex, NULL);

  struct timespec start_time, end_time;

  number_of_elements = atoi(arg[1]);
  nice1 = atoi(arg[2]);
  nice2 = atoi(arg[3]);
  
  block = mmap(NULL, number_of_elements*sizeof(int), PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  pthread_create(&th1, &attr, thread1, NULL);
  pthread_create(&th2, &attr, thread2, NULL);
  pthread_create(&th3, &attr, thread3, NULL);
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  clock_gettime(CLOCK_MONOTONIC, &end_time);

  double elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
  elapsed_time = elapsed_time/1000;

  sleep(1);
  int count = 0;
  if(number_of_elements>50){
    for(int i = 0; i<50; i++){
      printf("[%d]: %d\n",count+1, block[i]);
      count++;
    }
  }
  else{
    for(int i = 0; i<number_of_elements; i++){
      printf("[%d]: %d\n",count+1, block[i]);
      count++;
    }
  }
  printf("The war took %f seconds to end!\n",elapsed_time);
  return 0;
}

// Reviser Thread
void * thread1(){
  CPU_SET(0, &mask);

  int copy[number_of_elements];

  while(1){
    int count_minus1 = 0;
    int count_plus99 = 0;
    double percent_minus1;
    double percent_plus99;

    pthread_mutex_lock(&mutex);
    for(int i = 0; i<=number_of_elements; i++){
      copy[i] = block[i];
  
      if(copy[i]==-1){count_minus1++;}
      else if(copy[i]==99){count_plus99++;}
    }

    percent_minus1 = (count_minus1*100)/number_of_elements;
    percent_plus99 = (count_plus99*100)/number_of_elements;

    printf("Percent of -1: %f\n",percent_minus1);
    printf("Percent of 99: %f\n", percent_plus99);
    
    if(percent_minus1>=100 || percent_plus99>=100){
      stop = true;
      pthread_mutex_unlock(&mutex);
      break;
    }
    pthread_mutex_unlock(&mutex);
  }
}

// Thread that sets values to -1
void * thread2(){
  CPU_SET(1, &mask);
  nice(nice1);
  
  while(1){
    int random_index = rand() % number_of_elements;

    pthread_mutex_lock(&mutex);
    block[random_index] = -1;
    pthread_mutex_unlock(&mutex);

    if(stop){
      break;
    }
  }
}

// Thread that sets values to 99
void * thread3(){
  CPU_SET(1, &mask);
  nice(nice2);
  
  while(1){
    int random_index = rand() % number_of_elements;

    pthread_mutex_lock(&mutex);
    block[random_index] = 99;
    pthread_mutex_unlock(&mutex);

    if(stop){
      break;
    }
  }
}