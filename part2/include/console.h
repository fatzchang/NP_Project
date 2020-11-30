#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "remote.h"
#include <memory>
#include <boost/asio.hpp>

class console: public std::enable_shared_from_this<console> 
{
public:
    console(boost::asio::ip::tcp::socket socket, boost::asio::io_context &io_context);
    void start(std::string query_string);

private:
    boost::asio::io_context &ioc;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    int port_;
    std::string host_;
    std::string file_;
    std::map<int, std::shared_ptr<remote>> remote_list;
    void link_remote();
    void render();
};


#endif