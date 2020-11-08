#include "np_sinlge_proc.h"
#include "socket.h"
#include "npshell_proc.h"
#include "user.h"

#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <arpa/inet.h>
#include <queue>

int main(int argc, char *argv[]) {
    int msock;
    fd_set afds;
    fd_set rfds;
    struct sockaddr_in fsin;
    socklen_t alen;
    int nfds, fd;

    msock = passiveTCP(atoi(argv[1]), 5);
    // nfds = getdtablesize();
    nfds = 100; // FIXIT: getdtablesize is too large

    FD_ZERO(&afds);
    FD_SET(msock, &afds);

    while (1)
    {
        memcpy(&rfds, &afds, sizeof(rfds));
        if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0) {
            std::cerr << "select error" << std::endl;
        }

        if (FD_ISSET(msock, &rfds)) { // New connection
            int ssock;
            alen = sizeof(fsin);

            ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
            if (ssock < 0) {
                std::cerr << "ssock accept failed" << std::endl;
            }
            
            std::cout << "someone comes in!" << std::endl;
            std::cout << "ssock: " << ssock << std::endl;

            // initialize client
            
            char ip_c_string[INET_ADDRSTRLEN];
            
            inet_ntop(AF_INET, &fsin.sin_addr, ip_c_string, INET_ADDRSTRLEN);
            ulist::add(ssock, std::string(ip_c_string), fsin.sin_port);
            FD_SET(ssock, &afds);
        }
        // backup default std fd
        int stdin_tmp, stdout_tmp, stderr_tmp;
        stdin_tmp = dup(STDIN_FILENO);
        stdout_tmp = dup(STDOUT_FILENO);
        stderr_tmp = dup(STDERR_FILENO);

        for (fd = 0; fd < nfds; fd++) {
            if (fd != msock && FD_ISSET(fd, &rfds)) {
                if (npshell_proc(fd) == 1) {
                    close(fd);
                    FD_CLR(fd, &afds);
                }
            }
        }

        close(STDIN_FILENO);
        dup(stdin_tmp);
        close(STDOUT_FILENO);
        dup(stdout_tmp);
        close(STDERR_FILENO);
        dup(stderr_tmp);
        close(stdin_tmp);
        close(stdout_tmp);
        close(stderr_tmp);


        std::cout << "-----loop end------" << std::endl;
        
        
    }

    return 0;
}