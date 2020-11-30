#include "server.h"
#include "session.h"
#include <iostream>

server::server(boost::asio::io_context &io_context, short port):
    ioc(io_context),
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    do_accept();
}


void server::do_accept() {
    std::cout << "Accepting..." << std::endl;
    acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
        if (!ec) {
            std::cout << "Accept!" << std::endl;
    
            std::make_shared<session>(std::move(socket), ioc)->start();
            do_accept();
        } else {
            std::cout << "Accept error: " << ec.message() << std::endl;
            do_accept();
        }
        
    });
}