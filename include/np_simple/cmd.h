#ifndef __CMD_H
#define  __CMD_H

#include <string>
#include <vector>

class Cmd {

public:
    Cmd();
    void set_counter(int);
    void set_pipe_read(int);
    int get_counter();
    int get_pipe_read();
    char** get_command();
    void decrease_counter();
    void append(std::string token);
    void remove_last();
    ~Cmd();

private:
    int counter;
    int pipe_read;
    int pipe_write;
    std::vector<std::string> v;
};


#endif