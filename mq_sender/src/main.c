#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define QUEUE_NAME "/test_queue"
#define MAX_SIZE 1024
#define MSG_STOP "exit"

int main() {
  mqd_t mq;
  char buffer[MAX_SIZE];
  struct mq_attr attr;

  // Initialize the queue attributes
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_SIZE;
  attr.mq_curmsgs = 0;

  // Create the message queue
  mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
  if (mq == (mqd_t)-1) {
    perror("mq_open");
    exit(1);
  }

  // Send 5 messages
  for (int i = 1; i <= 50; ++i) {
    snprintf(buffer, MAX_SIZE, "%f", i * 0.3);

    if (mq_send(mq, buffer, MAX_SIZE, 0) == -1) {
      perror("mq_send");
      exit(1);
    }

    printf("Sent: %s\n", buffer);
  }

  // Send a stop message to the receiver
  if (mq_send(mq, MSG_STOP, MAX_SIZE, 0) == -1) {
    perror("mq_send");
    exit(1);
  }

  // Close the message queue
  if (mq_close(mq) == -1) {
    perror("mq_close");
    exit(1);
  }

  return 0;
}