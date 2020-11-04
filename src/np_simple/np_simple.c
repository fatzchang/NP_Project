#include "np_simple.h"
#include "socket.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  int msock, ssock, c_addr_len;
  struct sockaddr_in fsin;

  // printf("hello, your port is: %d\n", atoi(argv[1]));

  msock = passiveTCP(atoi(argv[1]), 5);
  while (1)
  {
    ssock = accept(msock, (struct sockaddr *)&fsin, &c_addr_len);
    // printf("Hello");

    if (ssock < 0) {
      // err, exit
    }

    pid_t pid = fork();
    if (pid == 0) {
      close(STDIN_FILENO);
      dup(ssock);
      close(STDOUT_FILENO);
      dup(ssock);
      close(STDERR_FILENO);
      dup(ssock);

      close(ssock);
      close(msock);

      execl("npshell", "npshell", (char *) NULL);

      exit(0);
    } else if (pid > 0) {
      close(ssock);
    }
  }
  

  return 0;
}
