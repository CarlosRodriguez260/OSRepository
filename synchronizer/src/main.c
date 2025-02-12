#include <time.h>
#include <stdio.h>
#include <mqueue.h>
#define QUEUE_NAME "/time_queue"

/**
 * @brief Creates a message queue which is sent data about the system's 
 * current time. This message queue is linked to another program which
 * will read the messages from the queue.
 * 
 * A message queue is created, and every second we will send information
 * about the system's localtime to the queue. This queue will be read in
 * another program and said program will display the information 
 * accodingly.
 * 
 * @note The instructions never specified how to launch these programs, or how to make
 * them end. We will then assume that each program runs for 20 seconds, sending or
 * receiving data between each other once "synchronizer" starts first and then 
 * "clock_plot" starts afterwards.
 * 
 * @note These programs, "synchronizer" and "clock_plot", should run at similar
 * or equal times. This one is "synchronizer", and should be running before
 * "clock_plot".
 * 
 * @return N/A
 */

int main(){
    // Taking precautions in case it somehow the queue already exists
    mq_unlink(QUEUE_NAME);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 9;
    attr.mq_msgsize = 8192;
    attr.mq_curmsgs = 0;

    mqd_t time_queue = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    int stop = 0;
    while(stop<20){
        // Get current time
        time_t now;
        struct tm *timeinfo;
        time(&now); // Get system time
        timeinfo = localtime(&now); // Convert to local time
        // Print current time
        // printf("Current Time: %02d:%02d:%02d\n", 
        // timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        char hour_buffer[8192];
        char min_buffer[8192];
        char sec_buffer[8192];
        snprintf(hour_buffer,8192,"h%02d",timeinfo->tm_hour);
        snprintf(min_buffer,8192,"m%02d",timeinfo->tm_min);
        snprintf(sec_buffer,8192,"s%02d",timeinfo->tm_sec);
        // Print out the buffer values to check if the
        // appropiate tag was appended
        printf("%s,%s,%s \n", hour_buffer, min_buffer, sec_buffer);

        // Send the data to the message queue from the
        // appropiate buffers
        mq_send(time_queue,hour_buffer,8192,0);
        mq_send(time_queue,min_buffer,8192,0);
        mq_send(time_queue,sec_buffer,8192,0);
        sleep(1);
        stop++;
    }
    return 0;
}