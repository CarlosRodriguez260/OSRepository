#include <time.h>
#include <stdio.h>
#include <mqueue.h>
#define QUEUE_NAME "/time_queue"

/**
 * @brief Reads a message queue which is sent data about the system's 
 * current time in another program, and displays it accordingly.
 * 
 * A message queue is read, and every second we will display the information
 * about the system's localtime from the queue.
 * 
 * @note These programs, "synchronizer" and "clock_plot", should run at similar
 * or equal times. This one is "clock_plot", and should run after "synchronizer"
 * starts.
 * 
 * @note The instructions never specified how to launch these programs, or how to make
 * them end. We will then assume that each program runs for 20 seconds, sending or
 * receiving data between each other once "synchronizer" starts first and then 
 * "clock_plot" starts afterwards.
 * 
 * @return N/A
 */

int main(){
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 9;
    attr.mq_msgsize = 8192;
    attr.mq_curmsgs = 0;

    mqd_t time_queue = mq_open(QUEUE_NAME, O_RDONLY, 0644, NULL);
    int stop = 0;
    while(stop<20){
        char hour[8192];
        char min[8192];
        char sec[8192];

        // Receive the data from the message queue and
        // store them in the appropiate buffers
        mq_receive(time_queue,hour,8192,NULL);
        mq_receive(time_queue,min,8192,NULL);
        mq_receive(time_queue,sec,8192,NULL);

        // Print out the time in <hour>:<min>:<sec> format
        printf("%s:%s:%s \n", hour+1, min+1, sec+1);
        sleep(1);
        stop++;
    }
    mq_unlink(QUEUE_NAME);
    return 0;
}
