#include "server.h"
#include "session2.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <sys/wait.h>
#include <memory>

#include <iostream>

using namespace boost::asio;

server::server(io_context &ioc, short port) 
    : ioc_(ioc), acceptor_(ioc), signal_(ioc, SIGCHLD)
{
    ip::tcp::endpoint endpoint(ip::tcp::v4(), port);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    do_accept();
}

void server::start_signal_wait()
{
    signal_.async_wait(boost::bind(&server::handle_signal_wait, this));
}

void server::handle_signal_wait() {
    if (acceptor_.is_open())
    {
      int status = 0;
      while (waitpid(-1, &status, WNOHANG) > 0) {}

      start_signal_wait();
    }
}

void server::do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec, ip::tcp::socket socket){
        if (!ec) {
            ioc_.notify_fork(boost::asio::io_context::fork_prepare);
            pid_t pid = fork();
            if (pid == 0) {
                ioc_.notify_fork(boost::asio::io_context::fork_child);
                acceptor_.close();
                signal_.cancel();
                std::make_shared<session>(std::move(socket), ioc_)->start();
            } else {
                socket.close();
                do_accept();
            } 
        } else {
            std::cerr << "Accept error: " << ec.message() << std::endl;
            do_accept();
        }
    });
}