#include <vector>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "../include/cmd.h"
#define MAX_ARG_SIZE (20)

Cmd::Cmd() {
    set_counter(0);
    set_pipe_read(-1);
}

void Cmd::append(std::string token) {
    v.push_back(token);
}

void Cmd::set_counter(int num) {
    counter = num;
}

int Cmd::get_counter() {
    return counter;
}

void Cmd::set_pipe_read(int fd) {
    pipe_read = fd;
}

int Cmd::get_pipe_read() {
   return pipe_read;
}

void Cmd::decrease_counter() {
    counter--;
}

// will add a NULL at the end
char** Cmd::get_command() {
    size_t size_with_null = v.size() + 1;
    
    char **t = (char **)malloc(sizeof(char *) * size_with_null);
    for (size_t i = 0; i < size_with_null; i++) {
        t[i] = (char *)malloc(sizeof(char) * (MAX_ARG_SIZE + 1));
    }

    for (size_t i = 0; i < v.size(); i++)
    {
        std::string token = v.at(i);
        memset(t[i], 0, MAX_ARG_SIZE + 1);
        token.copy(t[i], MAX_ARG_SIZE);
    }

    t[v.size()] = NULL;

    return t;
}

void Cmd::remove_last() {
    v.pop_back();
}



Cmd::~Cmd() {
    // free memorry
}