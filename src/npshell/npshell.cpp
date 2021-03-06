#include "npshell.h"
#include "utils.h"
#include "process.h"
#include "run.h"

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
    signal(SIGCHLD, psignal_handler);
    setenv("PATH", "bin:.", 1);
    vector<map<string, int>> num_pipe_list;
    vector<pid_t> pid_table;

    while(1) {
        string input;
        cout << "% ";
        getline(cin, input);
        stringstream ss(input);
        string token;
        vector<char*> cmd;
        int prev_pipe[2] = {-1, -1};
        
        if (input.length() != 0) {
            decrease_num_pipe(num_pipe_list);
        }

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
                    char *env_value = getenv(env.c_str());
                    if (env_value != NULL) {
                        cout << env_value << endl;
                    }
                }else {
                    cerr << "missing arguments" << endl;
                }
            }  else if (token == "|" || token == "!" || token == ">") {
                collect_zombie(pid_table);
                map<string, int> child_info = run_cmd(cmd, token, prev_pipe, false, num_pipe_list);
                
                // save recent pipe
                prev_pipe[0] = child_info.find("read")->second;
                prev_pipe[1] = child_info.find("write")->second;
                
                if (token == ">") {
                    waitpid(child_info.find("pid")->second, NULL, 0);
                    string filename;
                    if (ss >> filename) {
                        output(filename, prev_pipe[0]);
                        close(prev_pipe[0]);
                    }
                }else {
                    pid_table.push_back(child_info.find("pid")->second);
                }

                cmd.clear();
            } else if (token == "exit") {
                return 0;
            } else  {
                cmd.push_back(t); 
            }
        }
        collect_zombie(pid_table);
        // conver |number to int
        int pipe_counter = get_pipe_counter(token);
        bool is_num_pipe = (pipe_counter > 0) && (token.substr(0, 1) == "|" || token.substr(0, 1) == "!");

        if (is_num_pipe) {
            cmd.pop_back();
        }
        
        map<string, int> child_info = run_cmd(
            cmd, 
            token.substr(0, 1), 
            prev_pipe, 
            !is_num_pipe, 
            num_pipe_list
        ); // num pipe is not the last

        if (is_num_pipe) {    
            child_info.insert(pair<string, int>("counter", pipe_counter));
            num_pipe_list.push_back(child_info);
        }
   
        waitpid(child_info.find("pid")->second, NULL, 0);

        cmd.clear();

        // TODO: free malloc, combine with cmd.clear
    }

    return 0;
}
