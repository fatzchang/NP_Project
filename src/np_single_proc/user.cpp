#include "user.h"
#include "message.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>


user::user(int sockfd, int user_id, std::string ip_addr, in_port_t in_port) {
    fd = sockfd;
    ip = ip_addr;
    id = user_id;
    port = in_port;

    path = "bin:.";
    name = "(no name)";
}


// TODO: check exist
void user::change_name(std::string new_name) {
    if (name != new_name) {
        if (!ulist::change_name(name, new_name)) {
            std::string err_msg = "*** User '";
            err_msg += new_name;
            err_msg += "' already exists. ***\n";
            write(this->fd, err_msg.c_str(), err_msg.size());
            return;
        }

        name = new_name;
    }

    std::string broadcast_msg = change_name_msg(new_name, this->ip, std::to_string(this->port));
    ulist::broadcast(broadcast_msg.c_str(), broadcast_msg.size());  
}

int user::get_id() {
    return id;
}

int user::get_sockfd() {
    return fd;
}
std::string user::get_ip() {
    return ip;
}

std::string user::get_path() {
    return path;
}

void user::set_path(std::string new_path) {
    path = new_path;
}

in_port_t user::get_port() {
    return port;
}

void user::welcome() {
    std::string msg = welcome_msg();
    msg += login_msg(this->ip, std::to_string(this->port));
    msg += "% ";
    write(fd, msg.c_str(), msg.size());
}

void user::yell(std::string message) {
    std::string broadcast_msg = yell_msg(name, message);
    ulist::broadcast(broadcast_msg.c_str(), broadcast_msg.size());
}

void user::tell(int user_id, std::string message) {
    user *client = ulist::find_by_id(user_id);
    if (client == NULL) {
        std::cout << tell_fail_msg(user_id) << std::endl;
    } else {
        std::string msg = tell_msg(name, message);
        write(client->get_sockfd(), msg.c_str(), msg.size());
    }
}

user::~user() {
    ulist::remove(this);
    std::string broadcast_msg = logout_msg(name);
    ulist::broadcast(broadcast_msg.c_str(), broadcast_msg.size());
}



// ulist
std::queue<int> ulist::id_queue;
int ulist::max_id = 1;
std::set<std::string> ulist::name_set;
std::map<int , user *> ulist::fd_mapper;
std::map<int , user *> ulist::id_mapper;

void ulist::add(int ssock, std::string ip, in_port_t port) {
    int client_id;
    if (id_queue.empty()) {
        client_id = max_id;
        max_id++;    
    } else {
        client_id = id_queue.front();
        id_queue.pop();
    }
    user *client = new user(ssock, client_id, ip, port);
    client->welcome();
    
    // broadcast (exclude self)
    std::string login_broadcast = login_msg(client->get_ip(), std::to_string(client->get_port()));
    broadcast(login_broadcast.c_str(), login_broadcast.size());

    // add to list
    name_set.insert(client->name);
    fd_mapper.insert(std::pair<int, user *>(client->get_sockfd(), client));
    id_mapper.insert(std::pair<int, user *>(client->get_id(), client));
}

user * ulist::find_by_fd(int fd) {
    return fd_mapper.find(fd)->second;
}

user * ulist::find_by_id(int id) {
    std::map<int, user *>::iterator it;
    it = id_mapper.find(id);
    if (it == id_mapper.end()) {
        return NULL;
    }
    return it->second;
}

void ulist::broadcast(const char * message, size_t len) {
    std::map<int, user*>::iterator it;
    for (it = fd_mapper.begin(); it != fd_mapper.end(); it++) {
        write(it->first, message, len);
    }
}

bool ulist::name_exist(std::string name) {
    std::set<std::string>::iterator it;
    it = name_set.find(name);

    return it != name_set.end();
}
bool ulist::change_name(std::string old_name, std::string new_name) {
    if (name_exist(new_name)) {
        return false;
    }

    // remove old name
    std::set<std::string>::iterator it;
    it = name_set.find(old_name);
    if (it != name_set.end()) {
        name_set.erase(it);
    }

    // insert new name
    name_set.insert(new_name);

    return true;
}

void ulist::remove(user *client) {
    id_queue.push(client->get_id());
    std::map<int, user *>::iterator it;

    it = fd_mapper.find(client->get_sockfd());
    fd_mapper.erase(it);

    it = id_mapper.find(client->get_id());
    id_mapper.erase(it);
}