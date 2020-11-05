#ifndef __PROCESS_H
#define __PROCESS_H
#include <vector>
void psignal_handler(int);
void collect_zombie(std::vector<pid_t>&);

#endif