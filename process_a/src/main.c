#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <mqueue.h>
#include <signal.h>

// Global Variables
#define SHM_SIZE 25 * sizeof(float)
#define MSG_SIZE 30 * sizeof(char)
pid_t * processb_pid = NULL;
pid_t * processc_pid = NULL;
char MQ_NAME[8192];
char SHM_NAME[8192];
float * MEMORY_PTR;

// NOTE: The other two proccesses, Process B and Process C, 
// have to be built in their respective "build" folders. 
// For convenience, they are already pre-compiled.
// Just focus on building this one, Process A, and running it.
void handle_interrupt(int sig){
    munmap(MEMORY_PTR, SHM_SIZE);
    shm_unlink(SHM_NAME);

    mq_close(MQ_NAME);
    mq_unlink(MQ_NAME);

    kill(processb_pid,SIGINT);
    kill(processc_pid,SIGINT);

    printf("\nMemory closed, Queue unlinked, and proccesses stopped.\n");
    kill(getpid(),SIGKILL);
}

int main(int argc, char * argArray[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <MQ_NAME> <SHM_NAME>\n", argArray[0]);
        return 1;
    }

    sprintf(MQ_NAME,"/%s",argArray[1]);
    sprintf(SHM_NAME,"/%s",argArray[2]);

    // Create SHM
    int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(memory, SHM_SIZE);
    MEMORY_PTR = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    if(MEMORY_PTR == MAP_FAILED){
        perror("I fell! \n");
        exit(1);
    }

    //Create MQ
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;
    mqd_t MQ = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0644, &attr);

    pid_t parent = getpid();
    pid_t process_b = -1;
    pid_t process_c = -1;

    process_b = fork();
    if(getpid()==parent){
        process_c = fork();
    }

    if(process_b==0){
        char *argArrayB[] = {"../../process_b/build/process_b", MQ_NAME, SHM_NAME, NULL};
        execv(argArrayB[0],argArrayB);
        exit(0);
    }
    else if(process_c==0){
        char *argArrayC[] = {"../../process_c/build/process_c", SHM_NAME, NULL};
        execv(argArrayC[0],argArrayC);
        exit(0);
    }
    else{
        // For detection of interrupt signal in Process A
        struct sigaction sa;
        sa.sa_handler = handle_interrupt;
        sigaction(SIGINT, &sa, NULL);

        processb_pid = process_b;
        processc_pid = process_c;
        printf("Select one of the following:\n");
        while(1){
            int input;
            printf("1. Square Numbers\n2. Divide numbers in half\n3. Plot numbers\n");
            scanf("%d", &input);

            // Send a message to Process B to square all numbers in shared memory
            if(input==1){
                char message[MSG_SIZE];
                snprintf(message,"%s","Square");
                mq_send(MQ,message,MSG_SIZE,0);
            }
            else if(input==2){
                char message[MSG_SIZE];
                snprintf(message,"%s","half");
                mq_send(MQ,message,MSG_SIZE,0);
            }
            else if(input==3){
                kill(processc_pid,SIGUSR1);
            }
            else{
                printf("Invalid input...\n");
            }
            printf("\n");
        }
        wait(NULL);
        wait(NULL);
    }
    return 0;
}