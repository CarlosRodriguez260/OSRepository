#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t pid;

  pid = fork();

  printf("fork return value %d\n", pid);

  if (pid == 0) {
    execlp("firefox", "firefox", NULL);
  } else {
    printf("hi, I'm the parent, my id is %d\n", getpid());
    sleep(30);
  }

  printf("I'm done\n");

  return 0;
}
