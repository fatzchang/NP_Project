#include "console.h"
#include "remote.h"
// #include "renderer.h"

#include <memory>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

console::console(boost::asio::ip::tcp::socket socket, boost::asio::io_context &io_context) 
 : socket_(std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket))), 
 ioc(io_context)
{
        
}

void console::start(std::string query_string) 
{
    std::vector<std::string> splitVec;
    boost::algorithm::split(splitVec, query_string, boost::algorithm::is_any_of("&"));

    for (auto it = splitVec.begin(); it != splitVec.end(); it++) {
        size_t index = it->find("=");
        if ((it->size() - 1) > index) {
            size_t remote_index = stoi(it->substr(1,1));
            std::shared_ptr<remote> r(new remote(remote_index, ioc, socket_));


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
                r->port(value);
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
    std::cout << "before render" << std::endl;
    render();
}



void console::render() {
    auto self = shared_from_this();
    std::string page;
    page += "Content-type: text/html\r\n\r\n";
    page += "<!DOCTYPE html>";
    page += "<html lang=\"en\">";
    page +=     "<head>";
    page +=         "<meta charset=\"UTF-8\" />";
    page +=         "<title>NP Project 3 Sample Console</title>";
    page +=         "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\" integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\" crossorigin=\"anonymous\">";
    page +=     "</head>";
    page +=     "<body>";
    page +=         "<div class=\"container-fluid\">";
    page +=             "<div class=\"row\">";
    
    for (auto it = remote_list.begin(); it != remote_list.end(); it++) {

    page +=                 "<div class=\"col\" id=\"target_" + std::to_string(it->second->id()) + "\">";
    page +=                     "<div>" + it->second->host() + ":" + it->second->port() + "</div>";
    page +=                 "</div>";
        
    }

    page +=             "</div>";
    page +=         "</div>";
    

    page +=     "</body>";
    page += "</html>";

    boost::asio::async_write(*socket_, boost::asio::buffer(page), 
        [this, self](boost::system::error_code ec, size_t length){
            link_remote();
        }
    );
}

void console::link_remote() {
    std::cout << socket_->is_open() << std::endl;
    for (auto it = remote_list.begin(); it != remote_list.end(); it++) {
        it->second->connect();
    }
    std::cout << "connection done" << std::endl;
}
