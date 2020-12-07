#include "session.h"

#include <boost/asio.hpp>
#include <memory>
#include <stdio.h>
#include <arpa/inet.h>

#include <iostream>
using namespace boost::asio;

session::session(ip::tcp::socket socket, io_context &ioc)
    : client_socket_(std::move(socket)), ioc_(ioc), remote_socket_(ioc)
{

}

void session::start()
{
    do_read();
}

void session::do_read()
{
    auto self = shared_from_this();
    client_socket_.async_read_some(buffer(client_buffer_), [this, self](boost::system::error_code error, size_t length){
        std::cout << client_buffer_.data() << std::flush;
        if (is_connect()) {
            parse_request();
            remote_socket_.connect(ip::tcp::endpoint(ip::address(ip::address_v4(dst_ip_)), dst_port_));
            do_relay();
            // TODO: read remote socket
            display_info();
            reply();
        } else if (is_bind()) {

        } else {
            // send what received
            remote_socket_.write_some(buffer(client_buffer_, length));
        }
        do_read();
    });
}

bool session::is_connect()
{
    return (client_buffer_[0] == 4) && (client_buffer_[1] == 1);
}

bool session::is_bind()
{
    return (client_buffer_[0] == 4) && (client_buffer_[1] == 2);
}

void session::parse_request()
{
    std::cout << "parsing" << std::endl;
    cmd_ = client_buffer_[1];
    dst_port_ = (client_buffer_[2] << 8) | client_buffer_[3];
    userid_ = std::string(&client_buffer_[8]);

    if ((client_buffer_[4] | client_buffer_[5] | client_buffer_[6]) != 0) {
        for (size_t i = 0; i < 4; i++) {
            ((uint8_t *)(&dst_ip_))[i] = client_buffer_[i + 4];
        }
        dst_ip_ = htonl(dst_ip_); // convert to net endian
    } else {
        std::string domain = std::string(&client_buffer_[8 + strlen(&client_buffer_[8]) + 1]);
        dst_ip_ = fetch_ip(domain).to_uint();
    }
    std::cout << "parsed" << std::endl;
}

void session::display_info()
{
    printf("<S_IP>: %s\n", client_socket_.remote_endpoint().address().to_string().c_str());
    printf("<S_PORT>: %d\n", client_socket_.remote_endpoint().port());
    printf("<D_IP>: %s\n", ip_string().c_str());
    printf("<D_PORT>: %d\n", dst_port_);
    cmd_ == 1 ? printf("<Command>: CONNECT\n") : printf("<Command>: BIND\n");
    printf("<Reply>: Accept\n");
}

ip::address_v4 session::fetch_ip(std::string domain)
{
    ip::tcp::resolver resolver(ioc_);
    boost::system::error_code ec;
    ip::tcp::resolver::results_type endpoints = resolver.resolve(domain, std::to_string(dst_port_));

    return endpoints.begin()->endpoint().address().to_v4();
}

void session::reply()
{
    if (cmd_ == 1) {
        char msg[8];
        memset(msg, 0, 8);
        msg[0] = 0;
        msg[1] = 90;
        client_socket_.write_some(buffer(msg));
    }
}

std::string session::ip_string()
{
    std::string ip;
    uint32_t host_type_ip = ntohl(dst_ip_); // convert to host endian

    for (size_t i = 0; i < 4; i++) {
        ip += std::to_string(((uint8_t *)(&host_type_ip))[i]) + "." ;
    }
    ip.pop_back();
    return ip;
}

void session::do_relay() 
{
    auto self = shared_from_this();
    remote_socket_.async_read_some(buffer(remote_buffer_), [this, self](boost::system::error_code error, size_t length) {
        std::cout << remote_buffer_.data() << std::flush;
        client_socket_.write_some(buffer(remote_buffer_, length));
        do_relay();
    });
}