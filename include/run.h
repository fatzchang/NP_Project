#ifndef __RUN_H
#define __RUN_H

#include <map>
#include <vector>

using namespace std;

map<string, int> run_cmd(vector<char*> &, string , int, bool, vector<map<string, int>> &);

#endif