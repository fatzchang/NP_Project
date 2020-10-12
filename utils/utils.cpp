#include "utils.h"
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// pipe utils
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

// check file
// bool file_exist(string path) {
//     return access(path.c_str(), F_OK) == 0;
// }


// parse string to command vector
vector<string> parse_cmd(string cmd_string) {
    vector<string> v;
    stringstream ss(cmd_string);
    string token;

    while(ss>>token) {
        v.push_back(token.c_str());
    }

    v.push_back(NULL);

    return v;
}



