#include "remote.h"
#include <string>
#include <iostream>

remote::remote(int id, boost::asio::io_context &ioc) 
: id_(id), socket_(ioc), ioc_(ioc) {}

int remote::port() {
    return port_;
}

void remote::port(int port) {
    port_ = port;
}

std::string remote::host() {
    return host_;
}
void remote::host(std::string host) {
    host_ = host;
}

std::string remote::file() {
    return file_;
}

void remote::file(std::string file) {
    file_ = file;
}

int remote::id() {
    return id_;
}

void remote::connect() {
    boost::asio::ip::tcp::resolver resolver(ioc_);
    boost::asio::ip::tcp::resolver::iterator resolve_it = resolver.resolve(host(), std::to_string(port()));
    socket_.connect(resolve_it->endpoint());

    do_read_socket();
}

void remote::do_read_socket() {
    socket_.async_read_some(
        boost::asio::buffer(data_, DATA_MAX_LENGTH),
        [this](boost::system::error_code ec, size_t length){
            std::cout << data_ << std::endl;
        }
    );
}