#include "np_simple.h"
#include "socket.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
  int msock, ssock, c_addr_len;
  struct sockaddr_in fsin;

  // FIXIT: qlen
  msock = passiveTCP(SERVICE_PORT, 10);
  while (1)
  {
    ssock = accept(msock, (struct sockaddr *)&fsin, &c_addr_len);

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
