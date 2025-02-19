#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

// Global Variables
#define SHM_NAME "/memory"
#define SHM_SIZE 200 * sizeof(int)
#define SEMNAME_1 "/sem1"
#define SEMNAME_2 "/sem2"
float * MEMORY_PTR = NULL;

/**
 * @name Process B
 * 
 * @brief Subscribes to a shared memory of 200 integers, which program A
 * will initiate. Afterwards, we use semaphores between the programs to
 * increment the values of the memory by 1 in each program, five thousand times.
 * In other words, the programs will interchangeably increment all values in memory
 * by one 5000 times, and at the end all values of the memory should be equal to 10000.
 * 
 * @example
 * - Process A will begin incrementing while B waits, and afterwards Process A will wait 
 *   for Process B to increment. This will keep happening until both procceses incremented
 *   the values of the memory by 5000.
 * 
 * @note THIS IS THE PROCESS THAT SHOULD NOT BE RAN. THE ONE
 *       TO RUN IS IN THE "process_a/build" folder after building it.
 * 
 * @return N/A
 */

int main(){
    // Connect to Shared Memory started by "program_a"
    int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(memory, SHM_SIZE);
    int * memory_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    if(memory_ptr == MAP_FAILED){
        perror("I fell! \n");
        exit(1);
    }

    // Makes the global variable point to the shared memory,
    // allowing the handle functions to work with the memory.
    // Link the semaphores as well.
    MEMORY_PTR = memory_ptr;
    sem_t *sem1 = sem_open(SEMNAME_1, 0);
    sem_t *sem2 = sem_open(SEMNAME_2, 0);

    // This program has subscribed to the memory, we can post the first semaphore in program a
    printf("Program B subscribed to SHM, with code %d from semaphore \n",sem_post(sem1));
    
    int iterations = 5000;
    while(iterations!=0){
        // We have to either wait for the first iteration of 
        // increments to happen in program A or any subsequent increments.
        // Hence we make semaphore 2 stall program B.
        sem_wait(sem2);
        for(int i = 0; i<200; i++){
            memory_ptr[i] = memory_ptr[i]+1;
        }
        iterations--;
        // Once program B finishes incrementing, we post
        // semaphore 1 to make program A continue
        sem_post(sem1);
    }
    sem_unlink(sem1);
    sem_unlink(sem2);
    return 0;
}