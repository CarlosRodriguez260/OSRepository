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

/**
 * @name Process A
 * 
 * @brief Creates a shared memory of 200 integers, which program B
 * will subscribe to. Afterwards, we use semaphores between the programs to
 * increment the values of the memory by 1 in each program, five thousand times.
 * In other words, the programs will interchangeably increment all values in memory
 * by one 5000 times, and at the end all values of the memory should be equal to 10000.
 * 
 * @example
 * - Process A will begin incrementing while B waits, and afterwards Process A will wait 
 *   for Process B to increment. This will keep happening until both procceses incremented
 *   the values of the memory by 5000.
 * 
 * @note THIS IS THE PROCESS THAT SHOULD BE RAN, NOT THE ONE IN
 * "process_b" FOLDER. IF THIS PROGRAM IS RAN WITHOUT THE OTHER
 * ONE BEING BUILT, NO CHANGES WILL HAPPEN TO THE MEMORY.
 * 
 * @note For convenience, the "process_b" program is already pre-built and located.
 * 
 * @return N/A
 */

int main(){
    // Create a new shared memory
    int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(memory, SHM_SIZE);
    int * memory_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    if(memory_ptr == MAP_FAILED){
        perror("I fell! \n");
        exit(1);
    }
    printf("Program A created the shared memory (SHM)\n");

    // NOTE: The "process_b" program has to be built in the "/process_b/build" 
    // folder. For convenience, the program is already pre-built,
    // but in case of any errors, re-build it again in the specified folder.
    char *argArray[] = {"../../process_b/build/process_b", NULL};
    pid_t pid;
    pid = fork();
    if(pid==0){
        execv(argArray[0],argArray);
        exit(0);
    }
    else{
        sem_unlink(SEMNAME_1);
        sem_unlink(SEMNAME_2);
        sem_t *sem1 = sem_open(SEMNAME_1, O_CREAT, 0666, 0);
        sem_t *sem2 = sem_open(SEMNAME_2, O_CREAT, 0666, 0);
        // Make semaphore 1 wait for program B to subscribe to shared memory
        printf("Program A waited for Program B to subscribe to the SMH, with code %d from semaphore \n",sem_wait(sem1));

        // Now that Program B is subscribed to the memory, we start incrementing here
        int iterations = 5000;
        while(iterations!=0){
            for(int i = 0; i<200; i++){
                memory_ptr[i] = memory_ptr[i]+1;
            }
            iterations--;

            // Semaphore 2 is making program B stall, so we post it
            sem_post(sem2);
            // Now we make semaphore 1 stall program A
            sem_wait(sem1);
        }
        wait(NULL);
    }

    printf("Here are the values of the array: \n");
    int size = 0;
    for(int i = 0; i<200; i++){
        if(memory_ptr[i]!=10000){
            printf("Value %d was found at index %d of the memory, which is not 10000. \n", memory_ptr[i], i);
            exit(1);
        }

        if(i==199){
            printf("%d. ", memory_ptr[i]);
        }
        else{
            printf("%d,", memory_ptr[i]);
        }
        size++;
    }
    printf("There are %d elements in memory. We are done! \n",size);
    munmap(memory_ptr,SHM_SIZE);
    shm_unlink(SHM_NAME);
    return 0;
}
