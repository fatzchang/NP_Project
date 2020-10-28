#ifndef __CMD_H
#define  __CMD_H

#include <string>
#include <vector>

class Cmd {

public:
    Cmd();
    void set_counter(int);
    void append(std::string token);
    char** get_command();
    void remove_last();
    ~Cmd();
private:
    int counter;
    std::vector<std::string> v;
};


#endif