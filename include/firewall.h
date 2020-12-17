#ifndef __FIREWALL_H
#define __FIREWALL_H

#include "main.h"
#include <fstream>

class firewall {
public:
    static bool check(std::string ip, MODE mode);
private:
    static void load();
    static std::ifstream config;
};

#endif