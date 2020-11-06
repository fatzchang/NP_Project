#include "np_simple.h"
#include "socket.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <npshell_simple.h>

void reaper();

int main(int argc, char *argv[])
{
  // TODO: collect zombie
  // signal(SIGCHLD, reaper);
  int msock;
  int ssock;
  struct sockaddr_in fsin;
  socklen_t c_addr_len = sizeof((struct sockaddr *)&fsin);
  msock = passiveTCP(atoi(argv[1]), 5);
  printf("msock: %d\n", msock);
  while (1)
  {
    ssock = accept(msock, (struct sockaddr *)&fsin, &c_addr_len);
    if (ssock < 0) {
      perror("create ssock failed %d");
    }
    printf("someone comes in, ssock: %d\n", ssock);

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork failed");
    } if (pid == 0) {
      close(STDIN_FILENO);
      dup(ssock);
      close(STDOUT_FILENO);
      dup(ssock);
      close(STDERR_FILENO);
      dup(ssock);

      close(ssock);
      close(msock);

      exit(npshell_simple());
    } else {
      close(ssock);
    }
  }
  

  return 0;
}


void reaper() {
  union wait status;
  while(wait3(&status, WNOHANG, (struct rusage *)0) >= 0) {
    
  }
}