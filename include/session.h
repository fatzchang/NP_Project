#ifndef __SESSION_H
#define __SESSION_H
#include "main.h"

#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <memory>

using namespace boost::asio;

class session: public std::enable_shared_from_this<session>
{
public:
    session(ip::tcp::socket socket, io_context &ioc);
    void start();
private:
    ip::tcp::socket client_socket_;
    ip::tcp::socket remote_socket_;


    io_context &ioc_;
    int8_t cmd_;
    uint16_t dst_port_;
    uint32_t dst_ip_;
    std::string userid_;
    boost::array<char, MAX_BUFFER_SIZE> client_buffer_;
    boost::array<char, MAX_BUFFER_SIZE> remote_buffer_;

    void parse_request();
    void display_info();
    ip::address_v4 fetch_ip(std::string domain);
    void reply();
    std::string ip_string();
    void do_relay();
    void do_read();

    bool is_connect();
    bool is_bind();
};

#endif