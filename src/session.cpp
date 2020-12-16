#include "session.h"
#include "main.h"

#include <boost/asio.hpp>
#include <memory>
#include <stdio.h>
#include <arpa/inet.h>

#include <iostream>
#include <stdlib.h>


using namespace boost::asio;


session::session(ip::tcp::socket socket, io_context &ioc)
    : client_socket_(std::move(socket)), ioc_(ioc), remote_socket_(ioc), data_socket_(ioc), acceptor_(ioc_)
{

}

void session::start()
{
    do_read();
}

void session::do_read()
{
    auto self = shared_from_this();
    client_socket_.async_read_some(buffer(client_buffer_), [this, self](boost::system::error_code error, size_t length){
        if (length != 0) {
            if (is_connect()) {
                // connect command
                std::cout << "connect command" << std::endl;
                parse_request();
                remote_socket_.connect(ip::tcp::endpoint(ip::address(ip::address_v4(dst_ip_)), dst_port_));
                do_relay();
                display_info();
                reply(MODE::CONNECT);
            } else if (is_bind()) {
                // bind command
                std::cout << "bind command" << std::endl;
                parse_request();
                display_info();
                dst_port_ = get_unused_port();
                if (!data_socket_.is_open()) {
                    bind_op(dst_port_);
                    std::cout << "binded, start transfer..." << std::endl;
                }

                reply(MODE::BIND);
            } else {
                // send what received
                if (remote_socket_.is_open()){
                    boost::asio::write(remote_socket_, buffer(client_buffer_, length));
                    // remote_socket_.write_some(buffer(client_buffer_, length));
                }
            }

            do_read();
        } else {
            // std::cout << "close client socket" << std::endl;
            // client_socket_.close();
        }
    });
}

bool session::is_connect()
{
    return (client_buffer_[0] == 4) && (client_buffer_[1] == 1);
}

bool session::is_bind()
{
    return (client_buffer_[0] == 4) && (client_buffer_[1] == 2);
}

void session::parse_request()
{
    cmd_ = client_buffer_[1];
    dst_port_ = (client_buffer_[2] << 8) | ((unsigned char)client_buffer_[3]);
    userid_ = std::string(&client_buffer_[8]);

    if ((client_buffer_[4] | client_buffer_[5] | client_buffer_[6]) != 0) {
        for (size_t i = 0; i < 4; i++) {
            ((uint8_t *)(&dst_ip_))[i] = client_buffer_[i + 4];
        }
        dst_ip_ = htonl(dst_ip_); // convert to net endian
    } else {
        std::string domain = std::string(&client_buffer_[8 + strlen(&client_buffer_[8]) + 1]);
        dst_ip_ = fetch_ip(domain).to_uint();
    }
}

void session::display_info()
{
    printf("<S_IP>: %s\n", client_socket_.remote_endpoint().address().to_string().c_str());
    printf("<S_PORT>: %d\n", client_socket_.remote_endpoint().port());
    printf("<D_IP>: %s\n", ip_string().c_str());
    printf("<D_PORT>: %d\n", dst_port_);
    cmd_ == 1 ? printf("<Command>: CONNECT\n") : printf("<Command>: BIND\n");
    printf("<Reply>: Accept\n\n");
}

ip::address_v4 session::fetch_ip(std::string domain)
{
    ip::tcp::resolver resolver(ioc_);
    boost::system::error_code ec;
    ip::tcp::resolver::results_type endpoints = resolver.resolve(domain, std::to_string(dst_port_));

    return endpoints.begin()->endpoint().address().to_v4();
}

void session::reply(MODE mode)
{
    struct response {
        char vn = 0;
        char cd = 0;
        int16_t dst_port = 0;
        int32_t dst_ip = 0;
    };

    char msg[sizeof(response)];
    response res;
    res.vn = 0;
    res.cd = 90;

    if (mode == MODE::BIND) {
        res.dst_port = htons(dst_port_);
    }

    memcpy(msg, &res, sizeof(res));
    if (client_socket_.is_open()) {
        client_socket_.write_some(buffer(msg));
    } else {
        std::cout << "client_socket_ has closed" << std::endl;
    }
}

std::string session::ip_string()
{
    std::string ip;
    uint32_t host_type_ip = ntohl(dst_ip_); // convert to host endian

    for (size_t i = 0; i < 4; i++) {
        ip += std::to_string(((uint8_t *)(&host_type_ip))[i]) + "." ;
    }
    ip.pop_back();
    return ip;
}

void session::do_relay() 
{   
    auto self = shared_from_this();
    remote_socket_.async_read_some(buffer(remote_buffer_), [this, self](boost::system::error_code error, size_t length) {
        if (length != 0) {
            if (client_socket_.is_open()) {
                client_socket_.write_some(buffer(remote_buffer_, length));
            }
            do_relay();
        } else {
            remote_socket_.close();
        }
    });
}


void session::bind_op(int16_t port)
{   
    ip::tcp::endpoint endpoint(ip::tcp::v4(), port);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    acceptor_.async_accept([this](boost::system::error_code ec, ip::tcp::socket socket){
        if (!ec) {
            data_socket_ = std::move(socket);
            reply(MODE::BIND);
            do_relay_data();

        } else {
            std::cout << ec.message() << std::endl;
        }
    });
}


void session::do_relay_data() 
{   
    auto self = shared_from_this();
    data_socket_.async_read_some(buffer(data_buffer_), [this, self](boost::system::error_code error, size_t length) {
        std::cout << "transfered length: " << length << std::endl;
        
        if (client_socket_.is_open()) {
            int written_len = boost::asio::write(client_socket_, buffer(data_buffer_, length));
        }
        
        if (length == 0) {
            data_socket_.cancel();
            data_socket_.close();
        } else {
            do_relay_data();
        }
    });
}


uint16_t session::get_unused_port()
{
    int fd, r;
    struct sockaddr_in sa, bind_addr;
    socklen_t sa_len = sizeof(sa);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_addr.sin_port = htons(INADDR_ANY);
    bind(fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr));


    r = getsockname(fd, (struct sockaddr *)&sa, &sa_len);
    close(fd);
    
    return sa.sin_port;
}
