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
    std::cout << "start" << std::endl;
    // set env
    do_read(); 
}

void session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        boost::asio::buffer(data_),
        [this, self](boost::system::error_code ec, size_t length){
            std::cout << "read" << std::endl;
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

            _putenv_s("REQUEST_METHOD", (splitVec.at(0)).c_str()); // GET
            _putenv_s("REQUEST_URI", request_uri.c_str());
            _putenv_s("SERVER_PROTOCOL", (splitVec.at(2)).c_str()); // HTTP 1.1 ??
            // query string
            
            index = request_uri.find('?', 0);
            std::string query_string = boost::algorithm::trim_copy(request_uri.substr(index + 1));
            _putenv_s("QUERY_STRING", query_string.c_str()); // HTTP 1.1 ??
            
            cgi = boost::algorithm::trim_left_copy_if(
                request_uri.substr(0, index), 
                boost::algorithm::is_any_of("/")
            );
        }
        
        index = line.find(':', 0);
        if(index != std::string::npos) {
            std::string key = boost::algorithm::trim_copy(line.substr(0, index));
            std::string value = boost::algorithm::trim_copy(line.substr(index + 1));

            if (key == "Host") {
                _putenv_s("HTTP_HOST", value.c_str());
            }
        }

        is_first_line = false;
    }

    if (length == DATA_MAX_LENGTH) {
        do_read();
    } else {
        boost::asio::ip::tcp::endpoint server_endpoint(socket_.local_endpoint());
        boost::asio::ip::tcp::endpoint remote_endpoint(socket_.remote_endpoint());

        _putenv_s("SERVER_ADDR", server_endpoint.address().to_string().c_str());
        _putenv_s("SERVER_PORT", std::to_string(server_endpoint.port()).c_str());

        _putenv_s("REMOTE_ADDR", remote_endpoint.address().to_string().c_str());
        _putenv_s("REMOTE_PORT", std::to_string(remote_endpoint.port()).c_str());
        

        _close(0);
        _dup(socket_.native_handle());
        close(1);
        _dup(socket_.native_handle());

        socket_.close();
        

        std::cout << getenv("SERVER_PROTOCOL") << " 200 OK\r\n" << std::flush;

        // execute
        execlp((std::string("./") + cgi).c_str(), cgi.c_str(), NULL);
        std::perror("failed");
        std::cerr << cgi << std::endl;
        exit(0);
    }
}