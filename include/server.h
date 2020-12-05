#ifndef __SERVER_H
#define __SERVER_H

#include <boost/asio.hpp>

using namespace boost::asio;

class server {
public:
    server(io_context &ioc, short port);
private:
    io_context &ioc_;
    ip::tcp::acceptor acceptor_;
    signal_set signal_;

    void do_accept();
    void start_signal_wait();
    void handle_signal_wait();
};


#endif