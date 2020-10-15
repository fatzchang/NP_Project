#include "utils.h"
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

// pipe utils
// stdin -> pipe's read end
void link_pipe_read(int pipefd[2]) {
   close(STDIN_FILENO);
   dup(pipefd[0]);
}

// stdout -> pipe's write end
void link_pipe_write(int pipefd[2]) {
    close(STDOUT_FILENO);
    dup(pipefd[1]);
}


pid_t run_command(
    std::vector<char*> cmd, 
    int cmd_order, 
    int pipefd[2][2], 
    int pipe_counter) 
{
    if (cmd_order == FIRST_COMMAND || cmd_order == MEDIUM_COMMAND) {
        pipe(pipefd[pipe_counter]);
    }
    pid_t pid = fork();
    
    if (pid < 0) {
        std::cerr << "fork err" << std::endl;
    } else if (pid == 0) {
        if (cmd_order != ONLY_COMMAND) {
            if (cmd_order != FIRST_COMMAND) {
                // link stdin with previous pipe's read end
                link_pipe_read(pipefd[(pipe_counter + 1) % 2]);    
            }

            if (cmd_order != LAST_COMMAND) {
                // link stdout with newly created pipe's write end
                link_pipe_write(pipefd[pipe_counter]);
            }
        }

        execvp(cmd[0], &cmd[0]);
        std::string err_str;
        std::cerr << "Unknown Command: [" << cmd[0] << "]" << std::endl;
        exit(0);
    }

    return pid;
}
