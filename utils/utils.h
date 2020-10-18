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


pid_t output(string, int[2]);

void decrease_num_pipe(vector<map<string, int>> &);
void erase_num_pipe(vector<map<string, int>> &);

int get_pipe_counter(string);
int pipe_worker(vector<map<string, int>> &);
void replace_fd(int, int);
bool pipe_exist(vector<map<string, int>> &);

#endif