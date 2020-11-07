#ifndef __UTIL_H
#define __UTIL_H

#include "cmd.h"
#include <vector>
#include <unistd.h>
#include <string>
#include <map>

#define FIRST_COMMAND (1)
#define LAST_COMMAND (2)
#define MEDIUM_COMMAND (3)
#define ONLY_COMMAND (4)
#define BUF_SIZE (1024)

using namespace std;

void link_pipe_read(int);
void link_pipe_write(int, bool);


void output(string, int);

void decrease_num_pipe(vector<Cmd *> &);
void erase_num_pipe(vector<Cmd *> &);

int get_pipe_counter(string);
int pipe_worker(vector<Cmd *> &);
void replace_fd(int, int);
bool pipe_exist(vector<Cmd *> &);

void replace_fd(int fd);
void who(int fd);
#endif