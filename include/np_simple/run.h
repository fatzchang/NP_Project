#ifndef __RUN_H
#define __RUN_H

#include <vector>
#include <cmd.h>

using namespace std;

pid_t run_cmd(Cmd &, string , int, bool, vector<Cmd *> &);

#endif