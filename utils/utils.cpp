#include "utils.h"
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <sys/wait.h>


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



void decrease_num_pipe(vector<map<string, int>> &num_pipe_list) {
    for (size_t i = 0; i < num_pipe_list.size(); i++) {
        map<string, int>::iterator it = num_pipe_list.at(i).find("counter");
        (it->second)--;
    }
}

void erase_num_pipe(vector<map<string, int>> &num_pipe_list) {
    for (size_t i = 0; i < num_pipe_list.size(); i++) {
        map<string, int>::iterator it = num_pipe_list.at(i).find("counter");
        if (it->second == 0) {
            // close pipe both end
            close(num_pipe_list.at(i).find("read")->second);
            close(num_pipe_list.at(i).find("write")->second);
            num_pipe_list.erase(num_pipe_list.begin() + i);
            i--;
        }
    }
}


int get_pipe_counter(string token) {
    int pipe_counter = 0;

    try {
        pipe_counter = stoi(token.substr(1));
    } catch(const exception &e) {
        pipe_counter = -1;
    }

    return pipe_counter;
}


int pipe_worker(vector<map<string, int>> &num_pipe_list) {
    int pipefd[2];
    pipe(pipefd);
    // FIXIT: 有可能沒有輪到pipe, e.g cat但是當下沒有0的num pipe

    pid_t pid = fork();
    if (pid == 0) {
        replace_fd(STDOUT_FILENO, pipefd[1]);
        
        for (size_t i = 0; i < num_pipe_list.size(); i++) {
            if (num_pipe_list.at(i).find("counter")->second == 0) {
                int readfd = num_pipe_list.at(i).find("read")->second;
                int writefd = num_pipe_list.at(i).find("write")->second;
                
                string input;
                replace_fd(STDIN_FILENO, readfd);
                close(writefd);
                while (getline(cin, input)) {
                    cout << input << endl;
                }
                cin.clear();
            }
        }

        erase_num_pipe(num_pipe_list);
        exit(0);
    } else {
        erase_num_pipe(num_pipe_list); // erase then wait
        waitpid(pid, NULL, 0); //FIXIT
        close(pipefd[1]);
        
        // keep pipe read end
        return pipefd[0];
    }
}

void replace_fd(int ori, int targ) {
    close(ori);
    dup(targ);
    close(targ);
}