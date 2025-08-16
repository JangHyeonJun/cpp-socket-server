#include "Client.h"
#include <iostream>

Client::Client(boost::asio::io_context& io_context) 
    : socket_(io_context), io_context_(io_context) {
}

void Client::Connect(const std::string& host, unsigned short port) {
    auto self = shared_from_this();
    tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host, std::to_string(port));

    boost::asio::async_connect(socket_, endpoints,
        [this, self](boost::system::error_code ec, tcp::endpoint) {
            if (!ec) {
                std::cout << "[Client] Connected to server\n";
                DoRead();
            }
            else {
                std::cerr << "[Client] Connect failed: " << ec.message() << "\n";
            }
        });
}

void Client::Send(const std::string& message) {
    auto self = shared_from_this();
    boost::asio::post(io_context_, [this, self, message]() {
        bool write_in_progress = !write_messages_.empty();
        write_messages_.push_back(message);
        if (!write_in_progress) {
            DoWrite();
        }
        });
}

void Client::Close() {
    auto self = shared_from_this();
    boost::asio::post(io_context_, [this, self]() {
        socket_.close();
        });
}

void Client::DoRead() {
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, read_buffer_, '\n',
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::istream is(&read_buffer_);
                std::string line;
                std::getline(is, line);
                std::cout << "[Client] Received: " << line << "\n";
                DoRead();
            }
            else {
                std::cerr << "[Client] Read failed: " << ec.message() << "\n";
                socket_.close();
            }
        });
}

void Client::DoWrite() {
    auto self = shared_from_this();
    boost::asio::async_write(socket_,
        boost::asio::buffer(write_messages_.front() + "\n"),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                write_messages_.pop_front();
                if (!write_messages_.empty()) {
                    DoWrite();
                }
            }
            else {
                std::cerr << "[Client] Write failed: " << ec.message() << "\n";
                socket_.close();
            }
        });
}
