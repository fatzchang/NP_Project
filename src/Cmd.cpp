#include <vector>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "../include/cmd.h"
#define MAX_ARG_SIZE (20)

Cmd::Cmd() {
    set_counter(0);
}

void Cmd::append(std::string token) {
    v.push_back(token);
}

void Cmd::set_counter(int num) {
    counter = num;
}

char** Cmd::get_command() {
    v.push_back(NULL);

    char **t = (char **)malloc(sizeof(char *) * v.size());
    for (size_t i = 0; i < v.size(); i++) {
        t[i] = (char *)malloc(sizeof(char) * (MAX_ARG_SIZE + 1));
    }

    for (size_t i = 0; i < v.size(); i++)
    {
        std::string token = v.at(i);
        // FIXIT:
        // token.copy(, MAX_ARG_SIZE);
    }

    return t;
}

void Cmd::remove_last() {
    v.pop_back();
}

Cmd::~Cmd() {

}