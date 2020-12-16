#ifndef __DISPLAY
#define __DISPLAY

#include <stdint.h>
#include <string>

void display_info(const char *s_ip, uint16_t s_port, const char *d_ip, uint16_t d_port, int8_t cmd, bool stat);
std::string ip_string(uint32_t ip_int);

#endif