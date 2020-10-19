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

pid_t output(
    string filename,
    int prev_pipe_read
) {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "fork err" << endl;
    } else if (pid == 0) {
        replace_fd(STDIN_FILENO, prev_pipe_read);
        
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

    
    

    int stdout_tmp = dup(STDOUT_FILENO);
    replace_fd(STDOUT_FILENO, pipefd[1]);
    
    for (size_t i = 0; i < num_pipe_list.size(); i++) {
        if (num_pipe_list.at(i).find("counter")->second == 0) {
            char buf[100];
            while(read(num_pipe_list.at(0).find("read")->second, buf, 100) > 0) {
                cout << buf << endl;
            };
            // cerr << "pipe:" << "\n" << buf << endl;
            // cerr << "------" << endl;

            // int readfd = num_pipe_list.at(i).find("read")->second;
            // string input;
            // replace_fd(STDIN_FILENO, readfd);
            // cin.sync();
            // // cerr << readfd << endl;
            // cout << i << endl;
            // while (getline(cin, input)) {
            //     cout << input << endl;
            //     // cout << "@@@" << endl;
            // }
            // cin.clear();
        }
    }
    erase_num_pipe(num_pipe_list);
    replace_fd(STDOUT_FILENO, stdout_tmp);

    return pipefd[0];
}

void replace_fd(int ori, int targ) {
    close(ori);
    dup(targ);
    close(targ);
}

bool pipe_exist(vector<map<string, int>> &num_pipe_list) {
    bool exist = false;
    for (size_t i = 0; i < num_pipe_list.size(); i++)
    {
        if (num_pipe_list.at(i).find("counter")->second == 0) {
            exist = true;
            break;
        }
    }

    return exist;
}