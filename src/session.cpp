#include "session.h"

#include <boost/asio.hpp>
#include <memory>
// #include <iostream>
#include <stdio.h>

using namespace boost::asio;

session::session(ip::tcp::socket socket, io_context &ioc)
    : socket_(std::move(socket)), ioc_(ioc)
{
    parse_request();
    display_info();
}

void session::start()
{
    socket_.close();
}

void session::parse_request()
{
    int n = socket_.read_some(buffer(buffer_));
    cmd_ = buffer_[1];
    dst_port_ = buffer_[2] << 8 | buffer_[3];
    for (size_t i = 0; i < 4; i++)
    {
        dst_ip_[i] = buffer_[i + 4];
    }
    userid_ = std::string(&buffer_[8]);
    domain_ = std::string(&buffer_[8 + strlen(&buffer_[8]) + 1]);
}

void session::display_info()
{
    printf("<S_IP>: %s\n", socket_.remote_endpoint().address().to_string().c_str());
    printf("<S_PORT>: %d\n", socket_.remote_endpoint().port());
    printf("<D_IP>: %d.%d.%d.%d\n", dst_ip_[0], dst_ip_[1], dst_ip_[2], dst_ip_[3]);
    printf("<D_PORT>: %d\n", dst_port_);
    cmd_ == 1 ? printf("<Command>: CONNECT\n") : printf("<Command>: BIND\n");
}