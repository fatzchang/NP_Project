#include "utils.h"
#include "run.h"

#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <iostream>

using namespace std;

map<string, int> run_cmd(
    vector<char*> &cmd, 
    string token, 
    int prev_pipe[2], 
    bool is_last, 
    vector<map<string, int>> &num_pipe_list) 
{
    bool is_first = prev_pipe[0] < 0; // initial value is -1
    bool is_only = is_first && is_last;
    int pipefd[2];


    if (!is_only && !is_last) {
        pipe(pipefd);
    }

    cmd.push_back(NULL);

    pid_t pid = fork();

    if (pid == 0) {
        if (!is_only && !is_first) {
            link_pipe_read(prev_pipe);
        }
        if (!is_last) {
            link_pipe_write(pipefd, token == "!");
        }

        // child close previous pipefd+
        close(prev_pipe[0]);
        close(prev_pipe[1]);

        // deal with num pipe
        if (is_first && pipe_exist(num_pipe_list)) {
            int readfd = pipe_worker(num_pipe_list);
            replace_fd(STDIN_FILENO, readfd);
        }
        
        execvp(cmd[0], &cmd[0]);

        cerr << "Unknown command: [" << cmd[0] << "]." << endl;
        exit(0);
    } else {
        close(prev_pipe[0]);
        close(prev_pipe[1]);
        close(pipefd[1]);
        
        erase_num_pipe(num_pipe_list);

        map<string, int> m;

        m.insert(pair<string, int>("read", pipefd[0]));
        // m.insert(pair<string, int>("write", pipefd[1]));
        m.insert(pair<string, int>("pid", pid));

        

        return m;
    }
}