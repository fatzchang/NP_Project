#include <arpa/inet.h>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <vector>

#include "cmd.h"

class user {
public:
    std::string name;

    user(int sockfd, int id, std::string ip, in_port_t port);
    void change_name(std::string new_name);
    int get_id();
    int get_sockfd();
    std::string get_ip();
    std::string get_path();
    void set_path(std::string new_path);
    in_port_t get_port();
    void welcome();
    std::vector<Cmd *> num_pipe_list;

private:
    int id;
    std::string ip;
    std::string path; // default: bin:.
    in_port_t port;
    int fd;
    
};

class ulist{
public:
    static std::queue<int> id_queue;
    static int max_id;

    static std::map<int , user *> fd_mapper;
    static std::map<int , user *> id_mapper;
    static std::set<std::string> name_set;
    static void add(int ssock, std::string ip, in_port_t port);
    static user * find_by_fd(int fd);
    static void broadcast(const char * message, size_t len);
    static bool name_exist(std::string);
    static bool change_name(std::string old_name, std::string new_name);
};
