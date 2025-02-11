#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>

int main() 
{
  mqd_t queue1;
  queue1 = mq_open("/queue1", O_WRONLY, O_CREAT, 0664, NULL);

  mqd_t queue2;
  queue2 = mq_open("/queue2", O_RDWR);

  // queue, message to send, length of message, priority (higher is more important)
  mq_send(queue1,"HELLO",250000,0);

  
  return 0;
}