#ifndef __UTIL_H
#define __UTIL_H

#include <vector>
#include <unistd.h>
#include <string>
#include <map>

#define FIRST_COMMAND (1)
#define LAST_COMMAND (2)
#define MEDIUM_COMMAND (3)
#define ONLY_COMMAND (4)

using namespace std;

void link_pipe_read(int[2]);
void link_pipe_write(int[2], bool);

map<string, int> run_cmd(vector<char*> &, string , int[2], bool);
pid_t output(string, int[2]);

#endif