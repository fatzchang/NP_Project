#include "socket.h"
#include "np_simple.h"

#include <arpa/inet.h>
#include <string.h>

int passiveTCP(int port, int qlen)
{
  int s;
  struct sockaddr_in sin;

  // TODO: include
  bzero((char *)&sin, sizeof(sin));
  // fill in sock data structure
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);

  s = socket(PF_INET, SOCK_STREAM, 0);

  if (s < 0)
  {
    // err
  }

  if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    //err
  }

  if (listen(s, qlen) < 0)
  {
    // err
  }

  return s;
}