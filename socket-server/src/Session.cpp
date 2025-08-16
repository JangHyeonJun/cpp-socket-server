#include "Session.h"

#include <iostream>
#include <utility>
#include <boost/asio/write.hpp>

using boost::asio::ip::tcp;

Session::Session(tcp::socket socket)
    : socket_(std::move(socket)) {
}

void Session::Start() {
    DoRead();
}

void Session::Send(std::string message) {
    write_message_ = std::move(message);
    DoWrite();
}

void Session::DoRead() {
    auto self = shared_from_this();
    socket_.async_read_some(
        boost::asio::buffer(read_buffer_),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::string received(read_buffer_.data(), length);
                std::cout << "[Session] Received: " << received << "\n";
                
                Send(received + '\n'); // echo
                
                DoRead();
            }
            else {
                std::cout << "[Session] Connection closed: " << ec.message() << "\n";
            }
        });
}

void Session::DoWrite() {
    auto self = shared_from_this();
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(write_message_),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                std::cout << "[Session] Write error: " << ec.message() << "\n";
            }
        });
}
