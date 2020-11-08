#include "npshell_proc.h"
#include "utils.h"
#include "process.h"
#include "run.h"
#include "cmd.h"
#include "user.h"
#include "message.h"


#include <string>
#include <sstream>
#include <iostream>
#include <vector> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;


int npshell_proc(int fd) {
    replace_fd(fd);
    
    user * client = ulist::find_by_fd(fd);

    setenv("PATH", (client->get_path()).c_str(), 1);
    
    vector<pid_t> pid_table;
    string input;
    getline(cin, input);
    stringstream ss(input);
    string token;
    int prev_pipe_read = -1;
    
    // decrease if input is not empty
    if (input.length() != 0) {
        decrease_num_pipe(client->num_pipe_list);
    }

    Cmd *cmd = new Cmd();

    while(ss >> token) {
        if (token == "setenv") {
            string env, value;
            ss >> env;
            ss >> value;

            if (env.size() && value.size()) {
                client->set_path(value);
                // setenv(env.c_str(), value.c_str(), 1);
            } else {
                cerr << "missing arguments" << endl;
            }
        } else if (token == "printenv") {
            string env;
            ss >> env;
            if (env.size()) {
                char *env_value = getenv(env.c_str());
                if (env_value != NULL) {
                    cout << env_value << endl;
                }
            }else {
                cerr << "missing arguments" << endl;
            }
        }  else if (token == "|" || token == "!" || token == ">") {
            collect_zombie(pid_table);
            pid_t pid = run_cmd(*cmd, token, prev_pipe_read, false, client->num_pipe_list);
            
            // save recent pipe
            prev_pipe_read = cmd->get_pipe_read();
            
            if (token == ">") {
                waitpid(pid, NULL, 0);
                string filename;
                if (ss >> filename) {
                    output(filename, prev_pipe_read);
                    close(prev_pipe_read);
                }
            }else {
                pid_table.push_back(pid);
            }

            delete cmd;
            cmd = new Cmd();
        } else if (token == "exit") {
            delete client;
            return 1;
        } else if (token == "who") {
            who(fd);
        } else if (token == "name") {
            string name;
            ss >> name;
            if (name.size()) {
                client->change_name(name);
            }else {
                cerr << "missing arguments" << endl;
            }
        } else if (token == "yell") {
            string msg;
            ss >> msg;
            client->yell(msg);
        } else if (token == "tell") {
            int receiver;
            string tmp, msg;
            ss >> receiver;
            while(ss >> tmp) {
                msg += " ";
                msg += tmp;
            }
            client->tell(receiver, msg);
        } else  {
            cmd->append(token);
        }
    }
    collect_zombie(pid_table);
    // convert |number to int
    int pipe_counter = get_pipe_counter(token);
    bool is_num_pipe = (pipe_counter > 0) && (token.substr(0, 1) == "|" || token.substr(0, 1) == "!");

    if (is_num_pipe) {
        cmd->remove_last();
    }
    
    pid_t pid = run_cmd(
        *cmd, 
        token.substr(0, 1), 
        prev_pipe_read, 
        !is_num_pipe, 
        client->num_pipe_list
    ); // num pipe is not the last


    if (is_num_pipe) {
        cmd->set_counter(pipe_counter);
        client->num_pipe_list.push_back(cmd);
    }

    waitpid(pid, NULL, 0);
    cout << "% " << flush;

    return 0;
}


