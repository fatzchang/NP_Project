#include "utils.h"
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>


// pipe utils
// stdin -> pipe's read end
void link_pipe_read(int pipefd[2]) {
   close(STDIN_FILENO);
   dup(pipefd[0]);
}

// stdout -> pipe's write end
void link_pipe_write(int pipefd[2], bool pipe_err) {
    close(STDOUT_FILENO);
    dup(pipefd[1]);

    if (pipe_err) {
        close(STDERR_FILENO);
        dup(pipefd[1]);
    }
}

// return pipe read end
map<string, int> run_cmd(vector<char*> &cmd, string token, int prev_pipe[2], bool is_last) {
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

        // child close previous pipefd
        close(prev_pipe[0]);
        close(prev_pipe[1]);

        execvp(cmd[0], &cmd[0]);

        cerr << "Unknown Command: [" << cmd[0] << "]" << endl;
        exit(0);
    } else {
        map<string, int> m;

        m.insert(pair<string, int>("read", pipefd[0]));
        m.insert(pair<string, int>("write", pipefd[1]));
        m.insert(pair<string, int>("pid", pid));

        return m;
    }
}


pid_t output(
    string filename,
    int prev_pipe[2]
) {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "fork err" << endl;
    } else if (pid == 0) {
        link_pipe_read(prev_pipe);
        close(prev_pipe[0]);
        close(prev_pipe[1]);
        
        ofstream my_file(filename);
        string buffer;
        while (getline(cin, buffer)) {
            my_file << buffer << endl;
        }
        my_file.close();
        exit(0);
    }

    return pid;
}