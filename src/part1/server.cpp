#include "server.h"
#include "session.h"
#include <sys/wait.h>
#include <iostream>

server::server(boost::asio::io_context &io_context, short port):
    ioc(io_context),
    signal_(io_context, SIGCHLD),
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(ip::tcp::v4(), port)) {
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
            ioc.notify_fork(boost::asio::io_context::fork_prepare);
            pid_t pid = fork();
            if (pid == 0) {
                ioc.notify_fork(boost::asio::io_context::fork_child);
                acceptor_.close();
                signal_.cancel();

                std::make_shared<session>(std::move(socket))->start();
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