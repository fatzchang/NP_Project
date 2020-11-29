#include "session.h"
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <vector>
#include <io.h>

session::session(boost::asio::ip::tcp::socket socket)
 : socket_(std::move(socket)),
   is_first_line(true) 
{

}

void session::start() {
    do_read(); 
}

void session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        boost::asio::buffer(data_),
        [this, self](boost::system::error_code ec, size_t length){
            if (!ec) {
                parse(length);
            }
        }
    );
}

void session::parse(size_t length) {
    full_data_ << data_.c_array();
    std::string line;
    std::string::size_type index;
    while (std::getline(full_data_, line) && line != "\r") {
        std::cout << line << std::endl;
        if (is_first_line) {
            std::vector<std::string> splitVec;
            boost::algorithm::split(splitVec, line, boost::algorithm::is_any_of(" "), boost::algorithm::token_compress_on);

            REQUEST_METHOD = splitVec.at(0);
            REQUEST_URI = splitVec.at(1);
            SERVER_PROTOCOL = splitVec.at(2);
            
            index = REQUEST_URI.find('?', 0);
            QUERY_STRING = boost::algorithm::trim_copy(REQUEST_URI.substr(index + 1));
            
            cgi = boost::algorithm::trim_left_copy_if(
                REQUEST_URI.substr(0, index), 
                boost::algorithm::is_any_of("/")
            );
        }

        index = line.find(':', 0);
        if(index != std::string::npos) {
            std::string key = boost::algorithm::trim_copy(line.substr(0, index));
            std::string value = boost::algorithm::trim_copy(line.substr(index + 1));

            if (key == "Host") {
                HTTP_HOST = value;
            }
        }

        is_first_line = false;
    }

    if (length == DATA_MAX_LENGTH) {
        do_read();
    } else {
        boost::asio::ip::tcp::endpoint server_endpoint(socket_.local_endpoint());
        boost::asio::ip::tcp::endpoint remote_endpoint(socket_.remote_endpoint());
        SERVER_ADDR = server_endpoint.address().to_string();
        SERVER_PORT = std::to_string(server_endpoint.port());
        REMOTE_ADDR = remote_endpoint.address().to_string();
        REMOTE_PORT = std::to_string(remote_endpoint.port());


        // std::cout << SERVER_PROTOCOL << std::flush;
        // std::cout << " 200 OK\r\n" << std::endl;
    }
}