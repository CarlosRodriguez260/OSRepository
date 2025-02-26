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

#define SHM_SIZE 25 * sizeof(float)
#define MSG_SIZE 30 * sizeof(char)
char * MQ_NAME;
char * SHM_NAME;
float * MEMORY_PTR;

int main(int argc, char * names[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <MQ_NAME> <SHM_NAME>\n", names[0]);
        return 1;
    }

    MQ_NAME = names[1];
    SHM_NAME = names[2];
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;
    mqd_t MQ = mq_open(MQ_NAME, O_RDONLY, 0644, &attr);

    int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(memory, SHM_SIZE);
    MEMORY_PTR = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    if(MEMORY_PTR == MAP_FAILED){
        perror("I fell! \n");
        exit(1);
    }

    // Initialize values of SHM
    for(int i = 0; i<25; i++){
        MEMORY_PTR[i] = 2.0;
    }


    while(1){
        char message[MSG_SIZE];
        mq_receive(MQ,message,MSG_SIZE,NULL);
        
        if(strcmp("Square",message) == 0){
            for(int i = 0; i<25; i++){
                MEMORY_PTR[i] = MEMORY_PTR[i]*MEMORY_PTR[i];
            }
        }
        else if(strcmp("half",message) == 0){
            for(int i = 0; i<25; i++){
                MEMORY_PTR[i] = MEMORY_PTR[i]/2;
            }
        }
    }
    return 0;
}