#include "user.h"
#include <iostream>
#include <unistd.h>


user::user(int sockfd, int user_id, std::string ip_addr) {
    fd = sockfd;
    ip = ip_addr;
    id = user_id;

    path = "bin:.";
    name = "(No name)";
}


// TODO: check exist
void user::change_name(std::string new_name) {
    name = new_name;
}

int user::get_id() {
    return this->id;
}

int user::get_sockfd() {
    return this->fd;
}
std::string user::get_ip() {
    return this->ip;
}

std::string user::get_path() {
    return this->path;
}

void user::welcome() {
    char welcome_msg[] = "***************************************\n** Welcome to the information server **\n***************************************\n% ";
    write(this->fd, welcome_msg, sizeof(welcome_msg));
}

// ulist
std::set<std::string> ulist::name_set;
std::map<int , user *> ulist::fd_mapper;
std::map<int , user *> ulist::id_mapper;

void ulist::add(user *client) {
    std::string login_broadcast = "*** User ’(no name)’ entered from ";
    login_broadcast += client->get_ip();
    login_broadcast += ". ***";

    broadcast(login_broadcast.c_str(), login_broadcast.size());

    name_set.insert(client->name);
    fd_mapper.insert(std::pair<int, user *>(client->get_sockfd(), client));
    id_mapper.insert(std::pair<int, user *>(client->get_id(), client));
}

user * ulist::find_by_fd(int fd) {
    return fd_mapper.find(fd)->second;
}

void ulist::broadcast(const char * message, size_t len) {
    std::map<int, user*>::iterator it;
    for (it = fd_mapper.begin(); it != fd_mapper.end(); it++) {
        write(it->first, message, len);
    }
}