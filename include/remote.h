#ifndef __REMOTE_H
#define __REMOTE_H

#include <string>
#include <boost/asio.hpp>

class remote {
public:
    remote(int index, boost::asio::io_context &ioc);
    int port();
    void port(int port);
    void connect(boost::asio::io_context &ioc);

    std::string host();
    void host(std::string host);
    std::string file();
    void file(std::string filename);
private:
    int id_;
    int port_;
    std::string host_;
    std::string file_;
    boost::asio::ip::tcp::socket socket_;
};

#endif