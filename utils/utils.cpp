#include "utils.h"
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

#include <cstring>


// pipe utils
// stdin -> pipe's read end
void link_pipe_read(int pipefd[2]) {
   close(STDIN_FILENO);
   dup(pipefd[0]);
   close(pipefd[0]);
}

// stdout -> pipe's write end
void link_pipe_write(int pipefd[2], bool pipe_err) {
    close(STDOUT_FILENO);
    dup(pipefd[1]);
    if (pipe_err) {
        close(STDERR_FILENO);
        dup(pipefd[1]);
    }
    close(pipefd[1]);
}

pid_t run_command(
    std::vector<char*> cmd, 
    int cmd_order,
    int pipefd[2][2], 
    int pipe_counter, 
    bool pipe_err) 
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
                link_pipe_write(pipefd[pipe_counter], pipe_err);
            }
        }

        execvp(cmd[0], &cmd[0]);        

        // error handle
        std::string err_str;
        std::string input;
        while(std::getline(std::cin, input)) {
            std::cerr << input;
        }

        std::cerr << "Unknown Command: [" << cmd[0] << "]" << std::endl;
        exit(0);
    } else {
        // TODO: check if OK
        if (cmd_order != ONLY_COMMAND) {
            // close(pipefd[pipe_counter][0]);
            close(pipefd[pipe_counter][1]);

            if (cmd_order != FIRST_COMMAND) {
                close(pipefd[(pipe_counter+1)%2][0]);
            }
        }

        
        // close parent's previous pipefd
        // if(pipefd[(pipe_counter + 1) % 2][0] > 0) {
        //     close(pipefd[(pipe_counter + 1) % 2][0]);
        //     close(pipefd[(pipe_counter + 1) % 2][1]);
        // }
    }


    return pid;
}


pid_t output(
    std::vector<char*> cmd,
    std::string filename,
    int pipefd[2][2], 
    int pipe_counter
) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork err" << std::endl;
    } else if (pid == 0) {
        link_pipe_read(pipefd[(pipe_counter + 1) % 2]);
        std::ofstream my_file(filename);
        std::string buffer;
        while (std::getline(std::cin, buffer)) {
            my_file << buffer << std::endl;
        }
        my_file.close();
        exit(0);
    }

    return pid;
}