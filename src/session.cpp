#include "session.h"
#include "display.h"
#include "network.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

session::session(ip::tcp::socket socket, io_context &ioc)
 : ioc_(ioc), client_socket_(std::move(socket)), remote_socket_(ioc), data_socket_(ioc), acceptor_(ioc)
{

}

void session::start()
{
    client_socket_.read_some(buffer(client_buffer_));
    if (is_connect()) {
        std::cout << "connect command" << std::endl;
        do_connect();
        do_read();
        do_relay();
    } else if (is_bind()) {
        std::cout << "bind command" << std::endl;
        do_bind();
        do_read();
        do_relay();
    }
}

void session::do_connect()
{
    bool stat = parse();
    if (stat) {
        remote_socket_.connect(ip::tcp::endpoint(ip::address(ip::address_v4(dst_ip_)), dst_port_));
    }

    display_info(
        client_socket_.remote_endpoint().address().to_string().c_str(), 
        client_socket_.remote_endpoint().port(), 
        ip_string(dst_ip_).c_str(),
        dst_port_,
        1,
        stat
    );

    connect_reply(stat);
}

void session::do_bind()
{
    bool stat = parse();
    display_info(
        client_socket_.remote_endpoint().address().to_string().c_str(), 
        client_socket_.remote_endpoint().port(), 
        ip_string(dst_ip_).c_str(),
        dst_port_,
        2,
        stat
    );

    // get an unuse port
    bind_port_ = get_unused_port();
    // TODO: check port is ok

    if (stat) {
        do_listen();
    }
    
    bind_reply(stat);
}

void session::do_listen()
{
    ip::tcp::endpoint endpoint(ip::tcp::v4(), bind_port_);

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    acceptor_.async_accept([this](boost::system::error_code ec, ip::tcp::socket socket){
        if (!ec) {
            data_socket_ = std::move(socket);
            to_data = true;
            bind_reply(true);
            do_relay_data();
        } else {
            std::cout << ec.message() << std::endl;
        }
    });
}


void session::do_read()
{
    auto self = shared_from_this();
    client_socket_.async_read_some(buffer(client_buffer_), [this, self](boost::system::error_code error, size_t length){
        if (length != 0) {
            if (!to_data && remote_socket_.is_open()){
                boost::asio::write(remote_socket_, buffer(client_buffer_, length));
            }

            if (to_data && data_socket_.is_open()) {
                boost::asio::write(data_socket_, buffer(client_buffer_, length));
            }

            do_read();
        } else {
            if (to_data) {
                data_socket_.close();
            }
        }
    });
}


void session::do_relay() 
{   
    auto self = shared_from_this();
    remote_socket_.async_read_some(buffer(remote_buffer_), [this, self](boost::system::error_code error, size_t length) {
        if (length != 0) {
            if (client_socket_.is_open()) {
                // std::cout << "do_relay writing" << std::endl;
                boost::asio::write(client_socket_, buffer(remote_buffer_, length));
            }
            do_relay();
        }
    });
}

void session::do_relay_data()
{
    auto self = shared_from_this();
    data_socket_.async_read_some(buffer(data_buffer_), [this, self](boost::system::error_code error, size_t length) {
        if (client_socket_.is_open()) {
            boost::asio::write(client_socket_, buffer(data_buffer_, length));
        }
        
        if (length == 0) {
            client_socket_.close();
        } else {
            do_relay_data();
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

void session::bind_reply(bool stat)
{
    char msg[sizeof(struct response)];
    struct response res;
    res.cd = stat? 90 : 91;
    res.dst_port = htons(bind_port_);

    memcpy(msg, &res, sizeof(res));
    boost::asio::write(client_socket_, buffer(msg));
}

bool session::is_connect()
{
    return (client_buffer_[0] == 4) && (client_buffer_[1] == 1);   
}

bool session::is_bind()
{
    return (client_buffer_[0] == 4) && (client_buffer_[1] == 2);
}

bool session::parse()
{
    int8_t cmd = client_buffer_[1];
    dst_port_ = (client_buffer_[2] << 8) | ((unsigned char)client_buffer_[3]);
    bool success = false;

    if ((client_buffer_[4] | client_buffer_[5] | client_buffer_[6]) != 0) {
        for (size_t i = 0; i < 4; i++) {
            ((uint8_t *)(&dst_ip_))[i] = client_buffer_[i + 4];
        }
        dst_ip_ = htonl(dst_ip_); // convert to net endian

        success =  true;
    } else {
        std::string domain = std::string(&client_buffer_[8 + strlen(&client_buffer_[8]) + 1]);
        dst_ip_ = fetch_ip(domain, dst_port_).to_uint();
        // TODO:reject if failed
        success = true;
    }

    return success;
}