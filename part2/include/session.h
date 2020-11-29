#ifndef __SESSION_H
#define __SESSION_H
#define DATA_MAX_LENGTH (1024)

#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <sstream>


class session: public std::enable_shared_from_this<session> {
public:
    session(boost::asio::ip::tcp::socket socket);
    void start();

private:
    boost::asio::ip::tcp::socket socket_;
    boost::array<char, DATA_MAX_LENGTH> data_;
    std::stringstream full_data_;
    bool is_first_line;
    std::string cgi;
    
    void do_read();
    void parse(size_t length);
};




#endif