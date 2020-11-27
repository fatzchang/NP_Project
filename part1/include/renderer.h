#ifndef __RENDERER
#define __RENDERER

#include "remote.h"
#include <map>
#include <memory>

void render(std::map<int, std::shared_ptr<remote>> &remote_list);
void insert(int id, std::string text);

#endif