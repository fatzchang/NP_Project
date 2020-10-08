#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

string PATH = "bin/";

int main() {
    string input;
    string cmd;

    while(1) {
        cout << "% ";
        cin >> input;
        cmd = input.substr(0, input.find(" "));
        cout << cmd << endl;
        pid_t pid;

        if (pid = fork() < 0) {
            cout << "fork err" << endl;
            exit(0);
        } else {
            wait();
            cout << pid << endl;
            exit(0);
        }
    }
    return 0;
}