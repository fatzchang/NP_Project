#include <stdint.h>
#include <arpa/inet.h>
#include <unistd.h>

uint16_t get_unused_port()
{
    int fd, r;
    struct sockaddr_in sa, bind_addr;
    socklen_t sa_len = sizeof(sa);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_addr.sin_port = htons(INADDR_ANY);
    bind(fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr));


    r = getsockname(fd, (struct sockaddr *)&sa, &sa_len);
    close(fd);
    
    return sa.sin_port;
}
