#ifndef __SERVER_H
#define __SERVER_H

#include <memory>
#include <boost/asio.hpp>

using namespace boost::asio;

class server: public std::enable_shared_from_this<server>
{
public:
    server(io_context &io_context, short port);

private:
    ip::tcp::acceptor acceptor_;

    void do_accept();
};

#endif