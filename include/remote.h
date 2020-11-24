#ifndef __REMOTE_H
#define __REMOTE_H

#include "session.h"
#include <string>
#include <boost/asio.hpp>

class remote {
public:
    remote(int index, boost::asio::io_context &ioc);
    int port();
    void port(int port);
    std::string host();
    void host(std::string host);
    std::string file();
    void file(std::string filename);
    int id();

    void connect(boost::asio::io_context &ioc);
private:
    int id_;
    int port_;
    std::string host_;
    std::string file_;
    boost::asio::ip::tcp::socket socket_;
    char data_[DATA_MAX_LENGTH];

    void do_read_socket();
    void do_read_file();
};

#endif