#include "remote.h"
#include <string>
#include <iostream>
#include <memory>
#include <renderer.h>
#include <fstream>


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
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host(), std::to_string(port()));

    boost::asio::connect(socket_, endpoints);

    do_read_socket();
}

void remote::do_read_socket() {
    auto self(shared_from_this());
    socket_.async_read_some(
        boost::asio::buffer(data_),
        [this, self](boost::system::error_code ec, size_t length){
            if (ec) {
                throw boost::system::system_error(ec);
            }

            insert(id_, data_.data());
            std::string d(data_.data());
            data_.assign(0);

            if (d.find("%") != std::string::npos) {
                do_send();
            } else {
                do_read_socket();
            }
        }
    );
}


void remote::do_send() {
    auto self(shared_from_this());
    if (!f_s_.is_open()) {
        f_s_.open(file_);
    }
    getline(f_s_, tmp_from_file_);
    tmp_from_file_ += "\n";
    insert(id_, tmp_from_file_);

    boost::asio::async_write(socket_, buffer(tmp_from_file_), 
        [this, self](boost::system::error_code ec, size_t length) {
            if (ec) {
                throw boost::system::system_error(ec);
            }
            do_read_socket();
        }
    );


}