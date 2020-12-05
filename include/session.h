#ifndef __SESSION_H
#define __SESSION_H

#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace boost::asio;

class session
{
public:
    session(ip::tcp::socket socket, io_context &ioc);
    void start();
private:
    ip::tcp::socket socket_;
    io_context &ioc_;
    int8_t cmd_;
    uint16_t dst_port_;
    int8_t dst_ip_[4];
    std::string userid_;
    std::string domain_;
    boost::array<char, 256> buffer_;

    void parse_request();
    void display_info();
};

#endif