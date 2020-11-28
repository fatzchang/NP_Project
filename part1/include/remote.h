#ifndef __REMOTE_H
#define __REMOTE_H

#include "session.h"
#include <string>
#include <boost/asio.hpp>
#include <memory>
#include <fstream>

class remote: public std::enable_shared_from_this<remote>
{
public:
    remote(int index, boost::asio::io_context &ioc);
    int port();
    void port(int port);
    std::string host();
    void host(std::string host);
    std::string file();
    void file(std::string filename);
    int id();

    void connect();
private:
    int id_;
    int port_;
    std::string host_;
    std::string file_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::io_context &ioc_;
    boost::array<char, DATA_MAX_LENGTH> data_;
    std::string cat_data_;


    std::ifstream f_s_;
    std::string tmp_from_file_;

    void do_read_socket();
    void do_read_file();
    void do_send();
};

#endif