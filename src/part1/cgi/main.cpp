#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <map>

#include "renderer.h"
#include "remote.h"

void link_remote(std::map<int, remote*> &remote_list);

int main() {
    std::string q_str(getenv("QUERY_STRING"));
    std::vector<std::string> splitVec;
    boost::algorithm::split(splitVec, q_str, boost::algorithm::is_any_of("&"));
    std::map<int, remote*> remote_list;

    boost::asio::io_context io_context;


    for (auto it = splitVec.begin(); it != splitVec.end(); it++) {
        size_t index = it->find("=");
        if ((it->size() - 1) > index) {
            size_t remote_index = stoi(it->substr(1,1));
            remote *r = new remote(remote_index, io_context);

            auto remote_it = remote_list.find(remote_index);
            bool remote_exist = remote_it != remote_list.end();
            if (remote_exist) {
                r = remote_it->second;
            }

            std::string key = it->substr(0, 1);
            std::string value = it->substr(index + 1);
            if (key == "h") {
                r->host(value);
            } else if (key == "p") {
                r->port(stoi(value));
            } else if (key == "f") {
                r->file(value);
            }

            if (remote_exist) {
                remote_it->second = r;
            } else {
                remote_list.insert(std::pair<int, remote*>(remote_index, r));
            }
        }
    }

    
    render(remote_list);
    link_remote(remote_list);
    

    return 0;
}


void link_remote(std::map<int, remote*> &remote_list) {
    

    for (auto it = remote_list.begin(); it != remote_list.end(); it++) {
        // it->second.connect(io_context);
        // boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), )
    }
}