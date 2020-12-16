#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <map>

#include "renderer.h"
#include "remote.h"

void link_remote(std::map<int, std::shared_ptr<remote>> &remote_list, std::string &sh, std::string &sp);

int main() {
    std::string q_str(getenv("QUERY_STRING"));
    std::vector<std::string> splitVec;
    boost::algorithm::split(splitVec, q_str, boost::algorithm::is_any_of("&"));
    std::map<int, std::shared_ptr<remote>> remote_list;

    boost::asio::io_context io_context;
    std::string sh;
    std::string sp;

    for (auto it = splitVec.begin(); it != splitVec.end(); it++) {
        size_t index = it->find("=");
        if ((it->size() - 1) > index) {
            std::string column = it->substr(0, index);
            if (column == "sh") {
                sh = it->substr(index + 1);
            } else if (column == "sp") {
                sp = it->substr(index + 1);
            } else {
                size_t remote_index = stoi(it->substr(1,1));
                std::shared_ptr<remote> r(new remote(remote_index, io_context));

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
                    remote_list.insert(std::pair<int, std::shared_ptr<remote>>(remote_index, r));
                }
            }
        }
    }

    
    render(remote_list);
    link_remote(remote_list, sh, sp);
    
    io_context.run();

    return 0;
}


void link_remote(std::map<int, std::shared_ptr<remote>> &remote_list, std::string &sh,  std::string &sp) {
    bool use_sock = ((sh.size() > 0) && (sp.size() > 0));
    for (auto it = remote_list.begin(); it != remote_list.end(); it++) {
        if (use_sock) {
            it->second->connect(sh, sp);
        } else {
            it->second->connect();
        }
    }
}