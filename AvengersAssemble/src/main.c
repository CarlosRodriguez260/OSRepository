// Make a program that utilizes:
//  - Forking and Exec
//  - 2 children that will share messages between each other with MQ's
//  - 2 children that use a SHM between each other 
//  - Configure signals that start these processes on the parent
//  - Main takes two arguments, which will be the names of the processes 
//    with the MQ's and the SHM

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
#define SHM_SIZE 200 * sizeof(int)

// For dealing with INT signal
void handle_int(int sig){
    printf("My children must finish!\n");
}

int main(int argc, char *argArray[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <SHM_NAME> <MQ_NAME>\n", argArray[0]);
        return 1;
    }

    //If an interrupt signal is catched, deal with it
    struct sigaction sa;
    sa.sa_handler = handle_int;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGKILL, &sa, NULL);

    char SHM_NAME[100];
    char MQ_NAME[100];
    // Pre-append the backslash to the names
    sprintf(SHM_NAME, "/%s", argArray[1]);
    sprintf(MQ_NAME, "/%s", argArray[1]);

    pid_t parent = getpid();
    pid_t child1 = -1;
    pid_t child2 = -1;
    pid_t child3 = -1;
    pid_t child4 = -1;

    // Child 1 and 2 will focus on SHM
    // Child 3 and 4 will focus on MQ
    child1 = fork(); // Child 1
    if(parent==getpid()){
        child2 = fork(); // Child 2
    }
    if(parent==getpid()){
        child3 = fork(); // Child 3
    }
    if(parent==getpid()){
        child4 = fork(); // Child 4
    }

    if(child1==0){
        // Start the SHM
        int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ftruncate(memory, SHM_SIZE);
        int * memory_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
        if(memory_ptr == MAP_FAILED){
                perror("I fell! \n");
                exit(1);
        }

        for(int i = 0; i<200; i++){
            memory_ptr[i] = 69;
        }
        printf("[%d]: Memory Filled!\n", getpid());
        exit(0);
    }
    else if(child2==0){
        int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ftruncate(memory, SHM_SIZE);
        int * memory_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
        if(memory_ptr == MAP_FAILED){
                perror("I fell! \n");
                exit(1);
        }

        sleep(3); // Wait for child1 to fill the memory
        for(int i = 0; i<200; i++){
            if(i==199){
                printf("%d\n", memory_ptr[i]);
                break;
            }
            printf("%d,", memory_ptr[i]);
        }
        printf("[%d]: Memory Read! Closing SHM...\n", getpid());

        // Close the SHM
        munmap(memory_ptr, SHM_SIZE);
        shm_unlink(SHM_NAME);
        exit(0);
    }
    else if(child3==0){
        sleep(5); // Wait for other two childs
        printf("[%d]: I am awake, and feel like spreading joy!\n", getpid());
        sleep(2);

        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 9;
        attr.mq_msgsize = 8192;
        attr.mq_curmsgs = 0;

        char * m1 = "LOVE";
        char * m2 = "I";
        char * m3 = "YOU";
        mqd_t MQ = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0644, &attr);
        
        // Test out the priorities of MQ to see if 'I LOVE YOU' 
        // is sent correctly
        mq_send(MQ,m1,8192,1);
        mq_send(MQ,m2,8192,2);
        mq_send(MQ,m3,8192,0);
        printf("[%d]: Sending a love message to my brother!\n", getpid());
        exit(0);
    }
    else if(child4==0){
        sleep(10); // Wait for other two childs and brother

        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 9;
        attr.mq_msgsize = 8192;
        attr.mq_curmsgs = 0;
        mqd_t MQ = mq_open(MQ_NAME, O_RDONLY, 0644, &attr);

        char m1[8192];
        char m2[8192];
        char m3[8192];
        mq_receive(MQ,m1,8192,NULL);
        mq_receive(MQ,m2,8192,NULL);
        mq_receive(MQ,m3,8192,NULL);

        printf("[%d]: Following message received from my brother: %s %s %s\n", getpid(), m1, m2, m3);
        sleep(1);
        printf("[%d]: I love you too, brother...\n", getpid());
        mq_close(MQ_NAME);
        mq_unlink(MQ_NAME);
        exit(0);
    }
    else{
        wait(NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
        sleep(2);
        printf("[%d]: I am proud of you guys!\n", getpid());
    }
    return 0;
}