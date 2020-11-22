#include "session.h"

session::session(ip::tcp::socket socket)
 : socket_(std::move(socket)) {

 }

 void session::start() {
     do_read();
 }

 void session::do_read() {
     auto self(shared_from_this());
     socket_.async_read_some(
         buffer(data_, DATA_MAX_LENGTH),
         [this, self](boost::system::error_code ec, size_t length){
             if (!ec) {
                 do_write(length);
             }
         }
     );
 }

 void session::do_write(size_t length) {
     auto self(shared_from_this());
     async_write(
         socket_, buffer(data_, length),
         [this, self](boost::system::error_code ec, size_t length){
             if (!ec) {
                 do_read();
             }
         }
     );
 }