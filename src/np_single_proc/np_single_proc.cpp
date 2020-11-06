#include "socket.h"
#include "npshell_proc.h"

#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int msock;
    fd_set afds;
    fd_set rfds;
    struct sockaddr_in fsin;
    socklen_t alen;
    int nfds;

    msock = passiveTCP(atoi(argv[2]), 5);
    nfds = getdtablesize();

    FD_ZERO(&afds);
    FD_SET(msock, &afds);

    while (1)
    {
        memcpy(&rfds, &afds, sizeof(rfds));
        if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0) {
            std::cerr << "select error" << std::endl;
        }

        if (FD_ISSET(msock, &rfds)) {
            int ssock;
            alen = sizeof(fsin);

            ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
            if (ssock < 0) {
                std::cerr << "ssock accept failed" << std::endl;
            }

            FD_SET(ssock, &afds);
        }

        for (int fd = 0; fd < nfds; fd++) {
            if (fd != msock && FD_ISSET(fd, &rfds)) {
                npshell_proc(fd);
            }
        }
        
        
    }

    return 0;
}