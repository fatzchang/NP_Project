#include "main.h"
#include "remote.h"
#include "renderer.h"
#include "session.h"

#include <string>
#include <iostream>
#include <memory>
#include <fstream>


#include <stdio.h>

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

void remote::connect(std::string sh, std::string sp) {
    boost::asio::ip::tcp::resolver resolver(ioc_);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(sh, sp);

    boost::asio::connect(socket_, endpoints);

    char msg[sizeof(struct response)];
    struct response res;
    res.vn = 4;
    res.cd = 1;
    res.dst_ip = htonl(endpoints.begin()->endpoint().address().to_v4().to_uint());
    res.dst_port = htons((uint16_t) stoi(sp));

    memcpy(msg, &res, sizeof(res));
    boost::asio::write(socket_, buffer(msg));

    boost::array<char, MAX_BUFFER_SIZE> buf;
    socket_.read_some(buffer(buf));

    printf("response code: %d\n", buf[1]);
    if (buf[1] == 90) {
        do_read_socket();
    }
}

void remote::do_read_socket() {
    auto self(shared_from_this());
    data_.assign(0);
    socket_.async_read_some(
        boost::asio::buffer(data_),
        [this, self](boost::system::error_code ec, size_t length){
            if (ec) {
                std::cerr << boost::system::system_error(ec).what() << std::endl;
                socket_.close();
                return;
            }

            // if ()
            
            std::string data_string(data_.data());
            data_string = data_string.substr(0, length);
            cat_data_ += data_string;

            if (length < MAX_BUFFER_SIZE && ((cat_data_.back() == '\n') || data_string.find("%") != std::string::npos)) {
                if (cat_data_ != " ") {
                    insert(id_, cat_data_);
                }
                cat_data_.clear();
            }

            if (data_string.find("%") != std::string::npos) {
                do_send();
            }

            do_read_socket();
        }
    );
}


void remote::do_send() {
    auto self(shared_from_this());
    if (!f_s_.is_open()) {
        f_s_.open("./test_case/" + file_);
    }

    if (getline(f_s_, tmp_from_file_)) {
        tmp_from_file_ += "\n";
        insert(id_, tmp_from_file_);

        boost::asio::async_write(socket_, boost::asio::buffer(tmp_from_file_), 
            [this, self](boost::system::error_code ec, size_t length) {
                // do_read_socket();
                if (ec) {
                    std::cerr << boost::system::system_error(ec).what() << std::endl;
                }
            }
        );
    }
}