#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int originalPID = getpid();
    pid_t pid1, pid2 = -1, pid11;

    pid1 = fork();
    
    if (originalPID == getpid()) {
        pid2 = fork();
    }

    if (pid1 == 0) {
        printf("I am the first child. My PID should be +1 to my parent, and it is: %d\n", getpid());

        int originalPID2 = getpid();
        pid11 = fork();

        if (pid11 == 0) {
            printf("I am the child of child %d, and my PID is: %d\n", originalPID2, getpid());
            exit(0);
        }
        
        wait(NULL); // First child waits for its own child
        exit(0);
    } else if (pid2 == 0) {
        printf("I am the second child. My PID should be +2 to my parent, and it is: %d\n", getpid());
        exit(0);
    } else {
        printf("I am the parent. My PID is: %d\n", getpid());
        wait(NULL); // Wait for first child
        wait(NULL); // Wait for second child
    }

    return 0;
}
