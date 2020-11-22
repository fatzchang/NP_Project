#ifndef __SERVER_H
#define __SERVER_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


class server: public std::enable_shared_from_this<server>
{
public:
    server(boost::asio::io_context &io_context, short port);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::io_context &ioc;
    boost::asio::signal_set signal_;
    
    void start_signal_wait();
    void handle_signal_wait();
    void do_accept();
};

#endif