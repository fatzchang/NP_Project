#include <string>
#include <map>
#include <set>
#include <queue>

class user {
public:
    user(int sockfd, int id, std::string ip);
    std::string name;
    void change_name(std::string new_name);

    int get_id();
    int get_sockfd();
    std::string get_ip();
    std::string get_path();

    void welcome();
    

private:
    int id;
    std::string ip;
    std::string path; // default: bin:.
    int fd;
};

class ulist{
public:
    static std::queue<int> id_queue;
    static int max_id;

    static std::map<int , user *> fd_mapper;
    static std::map<int , user *> id_mapper;
    static std::set<std::string> name_set;
    static void add(int ssock, std::string ip);
    static user * find_by_fd(int fd);
    static void broadcast(const char * message, size_t len);
};
