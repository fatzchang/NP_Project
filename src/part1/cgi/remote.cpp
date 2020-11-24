#include "remote.h"
#include <string>
#include <iostream>

remote::remote(int id, boost::asio::io_context &ioc) : id_(id), socket_(ioc) {}

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

void remote::connect(boost::asio::io_context &ioc) {
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::resolver::iterator resolve_it = resolver.resolve(host(), std::to_string(port()));
    // std::cout << resolve_it->endpoint().address() << std::endl;
    socket_.connect(resolve_it->endpoint());
}