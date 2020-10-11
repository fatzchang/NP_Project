#include <string>
#include <iostream>
#include <regex>
#include <vector> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;

void link_pipe_read(int[2]);
void link_pipe_write(int[2]);
// vector<string> parse_cmd(string);

string PATH = "bin/";
regex r("\\s(\\||!)");

int main() {
    string input;
    string cmd;
    
    // represent previous pipe and newly created pipe
    int pipefd[2][2];
    int pipe_counter = 0; 

    // input = "ls -al | cat | number";
    string rmdstr;

    while(1) {
        cout << "% ";
        getline(cin, input);
        bool is_first = true;
        smatch s;
        
        cout << "enter string pass pipe:";
        getline(cin, rmdstr);
        
        while (regex_search(input, s, r))
        {
            cmd = input.substr(0, s.position());
            
            pipe(pipefd[pipe_counter]);
            pid_t pid = fork();

            if (pid < 0) {
                cout << "fork err" << endl;
            } else if (pid == 0) {
                // child process
                if (!is_first) {
                    // link stdin with previous pipe's read end
                    link_pipe_read(pipefd[(pipe_counter + 1) % 2]);
                    // vector<string> parsed_vector = parse_cmd(cmd);
                }
                // link stdout with newly created pipe's write end
                link_pipe_write(pipefd[pipe_counter]);

                // test
                if (is_first) {
                    cout << rmdstr << endl;
                } else {
                    string str_from_pipe;
                    getline(cin, str_from_pipe);
                    cout << str_from_pipe << endl;
                }

                exit(0);
            } else {
                // non-block waiting to prevent zombie process
                int stat;
                waitpid(pid, &stat, WNOHANG);

                pipe_counter = (pipe_counter + 1) % 2;
                input = input.substr(s.position() + (s.size() + 1));
                is_first = false;
            }
        }
        // last command or there's only one command
        pid_t pid = fork();
        if (pid < 0) {
            cout << "fork err" << endl;
        } else if (pid == 0) {
            if (!is_first) {
                link_pipe_read(pipefd[(pipe_counter + 1) % 2]);

                string str_from_pipe;
                getline(cin, str_from_pipe);
                cout << str_from_pipe << endl;
            }
            exit(0);
        } else {
            wait(NULL);
        }
    }
    return 0;
}

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

// vector<string> parse_cmd(string cmd_string) {
//     cout << cmd_string << endl;
//     vector<string> v = {"1"};

//     return v;
// }