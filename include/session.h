#ifndef __SESSION_H
#define __SESSION_H
#define DATA_MAX_LENGTH (1024)

#include <memory>
#include <boost/asio.hpp>


using namespace boost::asio;

class session: public std::enable_shared_from_this<session> {
public:
    session(ip::tcp::socket socket);
    void start();

private:
    ip::tcp::socket socket_;
    char data_[DATA_MAX_LENGTH];
    
    void do_read();
    void do_write(size_t length);
};



#endif