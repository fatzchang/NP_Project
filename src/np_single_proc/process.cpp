#include <sys/wait.h>
#include "process.h"
#include <vector>
#include <stdlib.h>

void psignal_handler(int signal) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {}
}

void collect_zombie(std::vector<pid_t>& pid_table) {
    for(size_t i = 0; i < pid_table.size(); i++) {
        int status;
        waitpid(pid_table.at(i), &status, WNOHANG);
        if (WIFEXITED(status)) {
            pid_table.erase(pid_table.begin() + i);
        }
    }
}