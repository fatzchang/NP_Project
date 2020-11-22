#include "server.h"
#include "session.h"

server::server(io_context &io_context, short port)
 : acceptor_(io_context, ip::tcp::endpoint(ip::tcp::v4(), port)) {
     do_accept();
}


void server::do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec, ip::tcp::socket socket){
        if (!ec) {
            std::make_shared<session>(std::move(socket))->start();
        }
    });
}