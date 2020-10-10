#include <string>
#include <iostream>
#include <regex>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;

string PATH = "bin/";
regex r("\\s(\\||!)");

int main() {
    string input;
    string cmd;
    int pipefd[2];
    bool isFirst = true;

    while(1) {
        cout << "% ";
        getline(cin, input);
        smatch s;

        while (regex_search(input, s, r))
        {
            cmd = input.substr(0, s.position());
            cout << "command: " << cmd << endl;
            input = input.substr(s.position() + (s.size() + 1));
        }

        // last command
        cout << "command: " << input << endl;
        
        // if (regex_search(input, s, r)) {
        //     cout << s.position() << endl;
        //     // for (int i = 0; i < s.size(); i++) {
        //     //     cout << i << ": " << s[i] << endl;
        //     // }
        // } else {
        //     cout << "not matched" << endl; 
        // }

        // not built-in command
        // while (input.find("|") != 0) {
        //     if (!isFirst) {
        //         // if not first command, close stdin
        //         close(STDIN_FILENO);
        //         dup(pipefd[0]);
        //     }
        //     close(STDOUT_FILENO);
        //     dup(pipefd[1]);
        //     isFirst = false;


        //     if (pipe(pipefd) < 0) {
        //     // error
        //         cout << "pipe err" << endl;
        //     } else {

        //     }
        // }
        
        // pid_t pid = fork();
        // if (pid  < 0) {
        //     cout << "fork err" << endl;
        // } else {
        //     if (pid == 0) {
        //         // child process
        //         execl((PATH + cmd).c_str(), "-l", NULL);
        //     } else {
        //         // parent process
        //         waitpid(pid, NULL, WNOHANG); // not block waiting
        //     }
        // }
    }
    return 0;
}