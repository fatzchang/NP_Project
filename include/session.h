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
    ip::tcp::acceptor acceptor_;
    ip::tcp::socket client_socket_;
    ip::tcp::socket remote_socket_;
    ip::tcp::socket data_socket_;

    boost::array<char, MAX_BUFFER_SIZE> client_buffer_;
    boost::array<char, MAX_BUFFER_SIZE> remote_buffer_;
    boost::array<char, MAX_BUFFER_SIZE> data_buffer_;

    uint32_t dst_ip_;
    uint16_t dst_port_;
    uint16_t bind_port_;

    bool to_data = false;

    /************ method ************/

    void do_connect();
    void do_bind();
    bool is_bind();
    bool is_connect();

    bool parse();
    
    void do_read();
    void do_relay();
    void do_relay_data();
    void do_listen();
    ip::address_v4 fetch_ip(std::string domain, uint16_t dst_port);
    void connect_reply(bool stat);
    void bind_reply(bool stat);
};

#endif