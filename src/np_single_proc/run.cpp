#include "utils.h"
#include "run.h"
#include "cmd.h"

#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <iostream>

using namespace std;

pid_t run_cmd(
    Cmd &cmd, 
    string token, 
    int prev_pipe_read, 
    bool is_last, 
    vector<Cmd *> &num_pipe_list) 
{
    bool is_first = prev_pipe_read < 0; // initial value is -1
    bool is_only = is_first && is_last;
    int pipefd[2] = {-1, -1};


    if (!is_only && !is_last) {
        pipe(pipefd);
    }

    pid_t pid;
    // wait for zombie collection if there are too many processes
    while ((pid = fork()) < 0) {
        usleep(1000);
    }

    if (pid == 0) {
        if (!is_only && !is_first) {
            link_pipe_read(prev_pipe_read);
        }
        if (!is_last) {
            link_pipe_write(pipefd[1], token == "!");
        }

        // deal with num pipe
        if (is_first && pipe_exist(num_pipe_list)) {
            int readfd = pipe_worker(num_pipe_list);
            replace_fd(STDIN_FILENO, readfd);
        }
        char ** cmd_arr = cmd.get_command();
        
        execvp(cmd_arr[0], cmd_arr);

        cerr << "Unknown command: [" << cmd_arr[0] << "]." << endl;
        exit(0);
    } else {
        close(prev_pipe_read);
        close(pipefd[1]); // close write end
        
        erase_num_pipe(num_pipe_list);

        cmd.set_pipe_read(pipefd[0]);
        
        return pid;
    }
}