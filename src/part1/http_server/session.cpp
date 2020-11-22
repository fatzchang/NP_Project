#include "session.h"
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <vector>
#include <stdio.h>

session::session(ip::tcp::socket socket)
 : socket_(std::move(socket)),
   is_first_line(true) 
{

}

void session::start() {
    // set env
    do_read();
}

void session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        buffer(data_),
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
        if (is_first_line) {
            std::vector<std::string> splitVec;
            boost::algorithm::split(splitVec, line, boost::algorithm::is_any_of(" "), boost::algorithm::token_compress_on);
            std::string request_uri = splitVec.at(1);

            setenv("REQUIEST_METHOD", (splitVec.at(0)).c_str(), 1); // GET
            setenv("REQUEST_URI", request_uri.c_str(), 1);
            setenv("SERVER_PROTOCOL", (splitVec.at(2)).c_str(), 1); // HTTP 1.1 ??
            // query string
            
            index = request_uri.find('?', 0);
            std::string query_string = boost::algorithm::trim_copy(request_uri.substr(index + 1));
            setenv("QUERY_STRING", query_string.c_str(), 1); // HTTP 1.1 ??
            
            cgi = boost::algorithm::trim_left_copy_if(request_uri, boost::algorithm::is_any_of("/"));
        }
        
        index = line.find(':', 0);
        if(index != std::string::npos) {
            std::string key = boost::algorithm::trim_copy(line.substr(0, index));
            std::string value = boost::algorithm::trim_copy(line.substr(index + 1));

            if (key == "Host") {
                setenv("HTTP_HOST", value.c_str(), 1);
            }
        }

        is_first_line = false;
    }

    if (length == DATA_MAX_LENGTH) {
        do_read();
    } else {
        boost::asio::ip::tcp::endpoint server_endpoint(socket_.local_endpoint());
        boost::asio::ip::tcp::endpoint remote_endpoint(socket_.remote_endpoint());

        setenv("SERVER_ADDR", server_endpoint.address().to_string().c_str(), 1);
        setenv("REMOTE_PORT", std::to_string(server_endpoint.port()).c_str(), 1);

        setenv("REMOTE_ADDR", remote_endpoint.address().to_string().c_str(), 1);
        setenv("REMOTE_PORT", std::to_string(remote_endpoint.port()).c_str(), 1);
        

        close(0);
        dup(socket_.native_handle());
        close(1);
        dup(socket_.native_handle());

        socket_.close();
        
        // execute
        execlp((std::string("./") + cgi).c_str(), cgi.c_str(), NULL);
        std::perror("failed");
        // std::cerr << "failed" << std::endl;
        exit(0);
    }
}