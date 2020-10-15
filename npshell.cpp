#include "npshell.h"
#include "./utils/utils.h"
#include "./utils/process.h"

#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <vector> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


using namespace std;

// TODO: clear parent process's pipe fd
int main() {
    // signal(SIGCHLD, psignal_handler);
    regex r("\\s(\\||\\!)");
    string input;
    string cmd;
    smatch s;
    // represent previous pipe and newly created pipe
    int pipefd[2][2];
    int pipe_counter = 0;
    int cmd_order = FIRST_COMMAND;

    setenv("PATH", "./bin", 1);

    while(1) {
        cout << "% ";
        getline(cin, input);
        stringstream ss(input);
        string token;
        vector<char*> cmd;
        vector<pid_t> pid_table;

        while(ss >> token) {
            char *t = (char *)malloc(sizeof(char)*10);
            for (size_t i = 0; i < 20; i++) {
                t[i] = '\0';
            }
            
            token.copy(t, token.size());

            if (token == "setenv") {
                string env, value;
                if (ss >> env && ss >> value) {
                    setenv(env.c_str(), value.c_str(), 1);
                } else {
                    cerr << "missing arguments" << endl;
                }
            } else if (token == "printenv") {
                string env;
                if (ss >> env) {
                    cout << getenv(env.c_str()) << endl;
                }else {
                    cerr << "missing arguments" << endl;
                }
            } else if (token == ">") {
                string filename;
                if (ss >> filename) {
                    
                }
            } else if (token == "|" || token == "!") {
                // deal with pipe
                cmd.push_back(NULL);
                collect_zombie(pid_table);
                pid_t child_pid = run_command(cmd, cmd_order, pipefd, pipe_counter);
                pid_table.push_back(child_pid);
                pipe_counter = (pipe_counter + 1) % 2;
                cmd_order = MEDIUM_COMMAND;
                cmd.clear();
            } else  {
                cmd.push_back(t); 
            }
        }

        if (cmd_order == FIRST_COMMAND) {
            // only one command
            cmd_order = ONLY_COMMAND;
        } else {
            // the last command
            cmd_order = LAST_COMMAND;
        }
        cmd.push_back(NULL);
        collect_zombie(pid_table);
        pid_t child_pid = run_command(cmd, cmd_order, pipefd, pipe_counter);
        waitpid(child_pid, NULL, 0);
        cmd_order = FIRST_COMMAND;

        // TODO: free malloc
    }
    return 0;
}

