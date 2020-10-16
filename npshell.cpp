#include "npshell.h"
#include "./utils/utils.h"
#include "./utils/process.h"

#include <string>
#include <sstream>
#include <iostream>
#include <vector> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <map>
#include <cstring>

using namespace std;

int main() {
    string input;
    setenv("PATH", "./bin", 1);
    vector<map<string, int>> num_pipe_info;

    while(1) {
        cout << "% ";
        getline(cin, input);
        stringstream ss(input);
        string token;
        vector<char*> cmd;
        vector<pid_t> pid_table;
        int prev_pipe[2] = {-1, -1};
        
        while(ss >> token) {
            char *t = (char *)malloc(sizeof(char)*(token.size()+1));
            memset(t, 0, sizeof(char) * (token.size()+1));
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
            } else if (token == "|" || token == "!" || token == ">") {
                collect_zombie(pid_table);
                map<string, int> child_info = run_cmd(cmd, token, prev_pipe, false);
                pid_table.push_back(child_info.find("pid")->second);
                // close previous pipe
                close(prev_pipe[0]);
                close(prev_pipe[1]);
                // save recent pipe
                prev_pipe[0] = child_info.find("read")->second;
                prev_pipe[1] = child_info.find("write")->second;
                cmd.clear();
                // insert counter

            } else if (token == "exit") {
                return 0;
            } else  {
                cmd.push_back(t); 
            }
        }

        collect_zombie(pid_table);
        map<string, int> child_info = run_cmd(cmd, "\0", prev_pipe, true);
        // if |number, push child_info to list
        close(prev_pipe[0]);
        close(prev_pipe[1]);
        // reset prev_pipe
        prev_pipe[0] = -1;
        prev_pipe[1] = -1;
        cmd.clear();
        waitpid(child_info.find("pid")->second, NULL, 0);

        // TODO: free malloc
    }

    return 0;
}