#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Global Variables
double arr1[1000000];
double arr2[1000000];
double result_arr[1000000];
int array_to_fill;
int operation;
double fill_number;
void* fillArray(void* param);
void* opArray(void* param);

/**
 * @brief Program that operates on two arrays using threads.
 * 
 * @details The program initializes 3 arrays of type double with
 * 1 million values each. Array 1 and 2 are filled up with values
 * inputted from the user, while Array 3, which is called the result array, stores
 * the resulting operations when either adding up or multiplying each element
 * from array 1 and array 2. These processes are done using threads, which operate
 * on different parts of the arrays to enable parallelization of the processes.
 * 
 * The threads call on two different functions, fillArray and opArray:
 * - Function fillArray fills up the specified array with the number from
 *   the user. Four threads divide the work by covering 250 thousand of the
 *   1 million numbers when filling up.
 * - Function opArray either sums up or multiplies the elements from array 1
 *  /**
 * @brief Program that operates on two arrays using threads.
 * 
 * @details The program initializes 3 arrays of type double with
 * 1 million values each. Array 1 and 2 are filled up with values
 * inputill be filled with 7's. The multiplication of each element from array 1 and 2 is 12,
 * hence the result array will be filled with 12's.
 * 
 * @note This version makes the multiplication run on a single thread.
 * 
 * @return N/A
 */
int main() {
  printf("Select one of the following:\n");
  while(1){
    int * input;
    printf("1. Fill Array 1\n2. Fill Array 2\n3. Add Arrays\n4. Multiply Arrays\n5. Plot result\n");
    scanf("%d",&input);

    if(input==1){
      // Fill Array 1. We ask for a number to fill array 1 with.
      printf("Type in a number to fill array 1 with\n");
      scanf("%lf",&fill_number);
      array_to_fill = 1;

      pthread_t thread_fill;
      pthread_attr_t attr;
      pthread_attr_init(&attr);

      // Call the 4 threads and join them afterwards.
      for(int i = 0; i<4; i++){
        pthread_create(&thread_fill,&attr,fillArray,(void*)i);
      }
      pthread_join(thread_fill,NULL);
 
      // for(int i = 0; i<20; i++){
      //   printf("%lf\n",arr1[i]);
      // }
    }
    else if(input==2){
      // Fill Array 2. We ask for a number to fill array 2 with.
      printf("Type in a number to fill array 2 with\n");
      scanf("%lf",&fill_number);
      array_to_fill = 2;

      pthread_t thread_fill;
      pthread_attr_t attr;
      pthread_attr_init(&attr);

      // Call the 4 threads and join them afterwards.
      for(int i = 0; i<4; i++){
        pthread_create(&thread_fill,&attr,fillArray,(void*)i);
      }
      pthread_join(thread_fill,NULL);

      // for(int i = 0; i<20; i++){
      //   printf("%lf\n",arr2[i]);
      // }
    }
    else if(input==3){
      // Operation equals 1 means addition.
      operation = 1;

      pthread_t thread_op;
      pthread_attr_t attr;
      pthread_attr_init(&attr);

      // Call the 4 threads and join them afterwards.
      for(int i = 0; i<4; i++){
        pthread_create(&thread_op,&attr,opArray,(void*)i);
      }
      pthread_join(thread_op,NULL);
    }
    else if(input==4){
      // Operation equals 2 means multiplication.
      operation = 2;

      pthread_t thread_op;
      pthread_attr_t attr;
      pthread_attr_init(&attr);

      // Use timespec and clock functions to get time elapsed
      // when multiplying the numbers.
      struct timespec start,end;
      clock_gettime(_POSIX_MONOTONIC_CLOCK,&start);

      // Call 1 thread here in multplication.
      pthread_create(&thread_op,&attr,opArray,NULL);
      pthread_join(thread_op,NULL);

      clock_gettime(_POSIX_MONOTONIC_CLOCK,&end);

      // Get the time elapsed and print it out
      double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      printf("Multiplying took %.5f seconds.\n",elapsed);
    }
    else if(input==5){
      // Print out the first 10 numbers of the result array
      for(int i = 0; i<10; i++){
        printf("[%d] : %lf\n", i+1, result_arr[i]);
      }
      printf("\n");
      sleep(1);
    }
    else{
      // Else for invalid inputs
      printf("Invalid input\n");
      sleep(1);
    }
  }
  return 0;
}

void* fillArray(void * param){
  // Starting point is used to determine where the function
  // starts and ends in between the 1 million spaces of the array.
  int starting_point = (int*)param;
  for(int i = 250000 * starting_point; i<(250000 * starting_point) +250000; i++){
    if(array_to_fill==1){
      // Fill array 1
      arr1[i] = fill_number;
    }
    else{
      // Fill array 2
      arr2[i] = fill_number;
    }
  }
}

void* opArray(void * param){
  // Starting point is used to determine where the function
  // starts and ends in between the 1 million spaces of the array.
  if(operation==2){
    // When multiplication is called, only 1 thread is created. 
    // Hence, we go from 0 to 1 million.
    for(int i = 0; i<1000000; i++){
      result_arr[i] = arr1[i]*arr2[i];
    }
  }
  else{
    int starting_point = (int*)param;
    for(int i = 250000 * starting_point; i<(250000 * starting_point) +250000; i++){
        if(operation==1){
          // Addition
          result_arr[i] = arr1[i] + arr2[i];
        }
      }
    }
}