#include "display.h"

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <arpa/inet.h>

void display_info(const char *s_ip, uint16_t s_port, const char *d_ip, uint16_t d_port, int8_t cmd, bool stat)
{
    printf("<S_IP>: %s\n", s_ip);
    printf("<S_PORT>: %d\n", s_port);
    printf("<D_IP>: %s\n", d_ip);
    printf("<D_PORT>: %d\n", d_port);
    cmd == 1 ? printf("<Command>: CONNECT\n") : printf("<Command>: BIND\n");
    if (stat) {
        printf("<Reply>: Accept\n\n");
    } else {
        printf("<Reply>: Reject\n\n");
    }
}


std::string ip_string(uint32_t ip_int)
{
    std::string ip;
    uint32_t host_type_ip = ntohl(ip_int); // convert to host endian

    for (size_t i = 0; i < 4; i++) {
        ip += std::to_string(((uint8_t *)(&host_type_ip))[i]) + "." ;
    }
    ip.pop_back();
    return ip;
}