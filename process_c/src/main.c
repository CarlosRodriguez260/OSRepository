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
char * SHM_NAME;
float * MEMORY_PTR;
int printer = 0;

void handle_usr1(int sig){
    printer = 1;
}

int main(int argc, char * names[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <SHM_NAME>\n", names[0]);
        return 1;
    }

    struct sigaction sa;
    sa.sa_handler = handle_usr1;
    sigaction(SIGUSR1, &sa, NULL);

    SHM_NAME = names[1];

    int memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(memory, SHM_SIZE);
    MEMORY_PTR = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    if(MEMORY_PTR == MAP_FAILED){
        perror("I fell! \n");
        exit(1);
    }

    while(1){
        if(printer==1){
            for(int i = 0; i<25; i++){
                if(i==24){
                    printf("%f.\n",MEMORY_PTR[i]);
                    break;
                }
                printf("%f,",MEMORY_PTR[i]);
            }
            printer = 0;
        }
        sleep(1);
    }
    return 0;
}