#include "npshell.h"
#include <string>
#include <iostream>
#include <regex>
#include <vector> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;
void run_command(string, int , int[2][2], int);


// TODO: clear parent process's pipe fd
int main() {
    string PATH = "bin/";
    regex r("\\s(\\||!)");
    string input;
    string cmd;
    smatch s;
    bool cmd_order = FIRST_COMMAND;
    
    // represent previous pipe and newly created pipe
    int pipefd[2][2];
    int pipe_counter = 0; 
   
    while(1) {
        cout << "% ";
        getline(cin, input);

        while (regex_search(input, s, r))
        {
            cmd = input.substr(0, s.position());
            run_command(cmd, cmd_order, pipefd, pipe_counter);
            pipe_counter = (pipe_counter + 1) % 2;
            input = input.substr(s.position() + (s.size() + 1));
            cmd_order = MEDIUM_COMMAND;
        }
        // last command or there's only one command
        cmd_order = LAST_COMMAND;
        run_command(input, cmd_order, pipefd, pipe_counter);
    }
    return 0;
}

void run_command(string cmd_string, int cmd_order, int pipefd[2][2], int pipe_counter) {
    pipe(pipefd[pipe_counter]);
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork err");
    } else if (pid == 0) {
        // child process
        if (cmd_order != FIRST_COMMAND) {
            // link stdin with previous pipe's read end
            link_pipe_read(pipefd[(pipe_counter + 1) % 2]);
        }
        if (cmd_order != LAST_COMMAND) {
            // link stdout with newly created pipe's write end
            link_pipe_write(pipefd[pipe_counter]);
        }


        // vector<string> parsed_vector = parse_cmd(cmd_string);
        // char *arg_arr[128][11];
        // for(size_t i = 0; i < parsed_vector.size(); i++) {
        //     parsed_vector.at(i).copy(*arg_arr[i], 10, 0);
        // }
        
        // execv("./bin/ls", *arg_arr);

        exit(0);
    } else {
        if (cmd_order == LAST_COMMAND) {
            // block waiting last command
            wait(NULL);
        } else {
            // non-block waiting medium command to prevent zombie process
            int stat;
            waitpid(pid, &stat, WNOHANG);
        }
    }
}