#ifndef __SESSION_H
#define __SESSION_H
#define DATA_MAX_LENGTH (1024)

#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <sstream>


class session: public std::enable_shared_from_this<session> {
public:
    session(boost::asio::ip::tcp::socket socket, boost::asio::io_context &io_context);
    void start();

private:
    boost::asio::io_context &ioc;
    boost::asio::ip::tcp::socket socket_;
    boost::array<char, DATA_MAX_LENGTH> data_;
    std::stringstream full_data_;
    bool is_first_line;
    std::string cgi;

    std::string REQUEST_METHOD;
    std::string REQUEST_URI;
    std::string SERVER_PROTOCOL;
    std::string QUERY_STRING;
    std::string SERVER_ADDR;
    std::string SERVER_PORT;
    std::string REMOTE_ADDR;
    std::string REMOTE_PORT;
    std::string HTTP_HOST;

    void exec_cgi();
    void do_read();
    void parse(size_t length);
    void do_read_console();
    void do_write_console();
};




#endif