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
#define SHM_SIZE 1000 * sizeof(float)
int COUNT = 0;
float * MEMORY_PTR = NULL;

// Function for handling SIGUSR1 signal
void handle_new(int sig) { 
    if(COUNT==1000){
        printf("Memory is full! \n");
    }
    else{
        if(COUNT==0){
            MEMORY_PTR[COUNT] = 1;
        }
        else{
            if(MEMORY_PTR[COUNT]!=0){
                COUNT++;
            }
            MEMORY_PTR[COUNT] = MEMORY_PTR[COUNT-1]*2;
        }
        COUNT++;
    }
}

// Function for handling SIGUSR2 signal
void handle_delete(int sig) { 
    if(COUNT==0){
        MEMORY_PTR[COUNT] = 0;
    }
    else{
        if(MEMORY_PTR[COUNT]==0){
            COUNT--;
        }
        MEMORY_PTR[COUNT] = 0;
    }
}

// Function for handling SIGALRM signal
void handle_divide(int sig) { 
    for(int i = 0; i<=COUNT; i++){
        MEMORY_PTR[i] = MEMORY_PTR[i]/2;
    }
}

// Function for handling SIGINT signal
void handle_stop(int sig){
    munmap(MEMORY_PTR, SHM_SIZE);
    shm_unlink(SHM_NAME);
    kill(getpid(), SIGTERM);
}

/**
 * @brief Connects to the shared memory created by "program_a", and
 * depending on the signals it receives from said program will execute
 * certain actions on the shared memory.
 * 
 * This program runs indefinitely until it receives the
 * appropiate signal for stopping. The parent, "program_a", 
 * is in charge of sending signals to this one, and here is 
 * what they mean here:
 * 
 * SIGUSR1: Adds a new number to the shared memory.
 *          Every new number is multiplied by 2.
 * SIGUSR2: Removes a number from the shared memory.
 * SIGALRM: Divides all numbers in the shared memory
 *          by 2.
 * SIGINT: Makes this program stop while cleaning
 *         out and unlinking the memory. The parent will 
 *         stop right after this as well.
 * 
 * Each of these signals have an appropiate handler function
 * designed to execute the instructions specified above.
 * 
 * @note THIS IS THE PROGRAM THAT SHOULD NOT BE RAN. THE ONE
 *       TO RUN IS IN THE "program_a" folder.
 * 
 * @return N/A
 */
int main(){
    // Connect to Shared Memory started by "program_a"
    int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(memory, SHM_SIZE);
    float * memory_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    if(memory_ptr == MAP_FAILED){
        perror("I fell! \n");
        exit(1);
    }
    // Makes the global variable point to the shared memory,
    // allowing the handle functions to work with the memory.
    MEMORY_PTR = memory_ptr;

    // Handler function setups for different signals
    struct sigaction sa;
    sa.sa_handler = handle_new;
    sigaction(SIGUSR1, &sa, NULL);

    struct sigaction sa1;
    sa1.sa_handler = handle_delete;
    sigaction(SIGUSR2, &sa1, NULL);

    struct sigaction sa2;
    sa2.sa_handler = handle_divide;
    sigaction(SIGALRM, &sa2, NULL);

    struct sigaction sa3;
    sa3.sa_handler = handle_stop;
    sigaction(SIGINT, &sa3, NULL);

    // Make program run indefinitely until
    // sent a signal to stop
    while (1) {
        sleep(1);
    }
    return 0;
}