#include <string>
#include <map>

class user {
public:
    void recieve();

private:
    int id;
    std::string name;
};

class user_list{
public:
    void send(int user_id);

private:
    std::map<int , user> list;    
};
