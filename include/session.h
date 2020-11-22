#ifndef __SESSION_H
#define __SESSION_H
#define DATA_MAX_LENGTH (1024)

#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>


using namespace boost::asio;

class session: public std::enable_shared_from_this<session> {
public:
    session(ip::tcp::socket socket);
    void start();

private:
    ip::tcp::socket socket_;
    boost::array<char, DATA_MAX_LENGTH> data_;
    
    void do_read();
    void do_write(size_t length);
};




#endif