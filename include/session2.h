#ifndef __SESSION_H
#define __SESSION_H

#include "main.h"

#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace boost::asio;

struct response {
    char vn = 0;
    char cd = 0;
    int16_t dst_port = 0;
    int32_t dst_ip = 0;
};

class session: public std::enable_shared_from_this<session>
{
public:
    session(ip::tcp::socket socket, io_context &ioc);
    void start();
private:
    io_context &ioc_;
    ip::tcp::socket client_socket_;
    ip::tcp::socket remote_socket_;

    boost::array<char, MAX_BUFFER_SIZE> client_buffer_;
    boost::array<char, MAX_BUFFER_SIZE> remote_buffer_;

    bool connected = false;

    /************ method ************/

    void connect();
    void do_read();
    void do_relay();
    ip::address_v4 fetch_ip(std::string domain, uint16_t dst_port);
    void connect_reply(bool stat);
};

#endif