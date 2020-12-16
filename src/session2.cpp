#include "session2.h"
#include "display.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

session::session(ip::tcp::socket socket, io_context &ioc)
 : ioc_(ioc), client_socket_(std::move(socket)), remote_socket_(ioc)
{

}

void session::start()
{
    if (!connected) {
        connect();
        do_relay();
        start();
    } else {
        do_read();
    }
    
}

void session::connect()
{
    client_socket_.read_some(buffer(client_buffer_));

    uint32_t dst_ip;
    int8_t cmd = client_buffer_[1];
    uint16_t dst_port = (client_buffer_[2] << 8) | ((unsigned char)client_buffer_[3]);
    std::string userid = std::string(&client_buffer_[8]);

    bool stat = false;

    if ((client_buffer_[4] | client_buffer_[5] | client_buffer_[6]) != 0) {
        for (size_t i = 0; i < 4; i++) {
            ((uint8_t *)(&dst_ip))[i] = client_buffer_[i + 4];
        }
        dst_ip = htonl(dst_ip); // convert to net endian

        stat =  true;
    } else {
        std::string domain = std::string(&client_buffer_[8 + strlen(&client_buffer_[8]) + 1]);
        dst_ip = fetch_ip(domain, dst_port).to_uint();
        // TODO:reject if failed
        stat = true;
    }

    remote_socket_.connect(ip::tcp::endpoint(ip::address(ip::address_v4(dst_ip)), dst_port));

    display_info(
        client_socket_.remote_endpoint().address().to_string().c_str(), 
        client_socket_.remote_endpoint().port(), 
        ip_string(dst_ip).c_str(),
        dst_port,
        cmd,
        stat
    );

    connect_reply(stat);
    connected = stat;
}


void session::do_read()
{
    auto self = shared_from_this();
    client_socket_.async_read_some(buffer(client_buffer_), [this, self](boost::system::error_code error, size_t length){
        if (length != 0) {
            if (remote_socket_.is_open()){
                int w = boost::asio::write(remote_socket_, buffer(client_buffer_, length));
            }
            do_read();
        } else {
            // std::cout << "close client socket" << std::endl;
            // client_socket_.close();
        }
    });
}



ip::address_v4 session::fetch_ip(std::string domain, uint16_t dst_port)
{
    ip::tcp::resolver resolver(ioc_);
    boost::system::error_code ec;
    ip::tcp::resolver::results_type endpoints = resolver.resolve(domain, std::to_string(dst_port));

    return endpoints.begin()->endpoint().address().to_v4();
}

void session::connect_reply(bool stat) 
{
    char msg[sizeof(struct response)];
    struct response res;
    res.cd = stat? 90 : 91;

    memcpy(msg, &res, sizeof(res));
    boost::asio::write(client_socket_, buffer(msg));
}

 void session::do_relay() 
{   
    auto self = shared_from_this();
    remote_socket_.async_read_some(buffer(remote_buffer_), [this, self](boost::system::error_code error, size_t length) {
        if (length != 0) {
            if (client_socket_.is_open()) {
                boost::asio::write(client_socket_, buffer(remote_buffer_, length));
            }
            do_relay();
        } else {
            // std::cout << "close remote socket" << std::endl;
            // remote_socket_.close();
        }
    });
}