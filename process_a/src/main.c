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

/**
 * @brief Creates a shared memory called "/memory" and forks 
 * another program called "program_b", which receives signals
 * from this program to perform certain actions on the shared memory.
 * 
 * The forked program runs indefinitely until it receives the
 * appropiate signal for stopping. The parent is in charge of
 * sending the signals, and here is what they send per input:
 * 
 * Input 1 -> SIGUSR1: Adds a new number to the shared memory.
 *                     Every new number is multiplied by 2.
 * Input 2 -> SIGUSR2: Removes a number from the shared memory.
 * Input 3 -> SIGALRM: Divides all numbers in the shared memory
 *                     by 2.
 * Input 4 -> No signal, just prints values from shared memory
 * Input 5 -> SIGINT: Makes the child program stop while cleaning
 *                    out and unlinking the memory. The parent 
 *                    will stop right after this as well.
 * 
 * @note THIS IS THE PROGRAM THAT SHOULD BE RAN, NOT THE ONE IN
 * "process_b" FOLDER. IF THIS PROGRAM IS RAN WITHOUT THE OTHER
 * ONE BEING BUILT, NO CHANGES WILL HAPPEN TO THE MEMORY.
 * 
 * @note For convenience, the "program_b" program is already pre-built.
 * 
 * @return N/A
 */

int main(){
    // Create a new shared memory
    int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(memory, SHM_SIZE);
    float * memory_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    if(memory_ptr == MAP_FAILED){
        perror("I fell! \n");
        exit(1);
    }

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
        printf("Input one of the following: \n");
        while(1){
            // 1 = SIGUSR1
            // 2 = SIGUSR2 
            // 3 = SIGALRM
            // 4 = Print Numbers in Shared Memory
            // 5 = Stop the program

            //printf("%d \n", pid);
            int choice = 0;
            printf("1 = Add number to memory; 2 = Delete number from memory\n3 = Divide all memory numbers by 2; 4 = Print numbers in memory\n5 = Terminate\n");
            scanf("%d", &choice);

            if(choice==1){kill(pid, SIGUSR1);}
            else if(choice==2){kill(pid, SIGUSR2);}
            else if(choice==3){kill(pid, SIGALRM);}
            else if(choice==4){
                printf("Memory: \n");
                for(int i = 0; i<1000; i++){
                    if(memory_ptr[i]==0){break;}
                    printf("%f\n",memory_ptr[i]);
                }
            }
            else if(choice==5){
                kill(pid, SIGINT);
                break;
            }
            else{
                printf("Incorrect input...\n");
                sleep(1);
            }
            printf("\n");
        }
        wait(NULL);
    }
    return 0;
}
