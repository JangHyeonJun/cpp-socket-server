#include "Client.h"
#include "Util/StringUtil.h"

#include <iostream>

#define CLIENT_HANDLE_ERROR(ec) HandleError(ec, __func__, __FILE__, __LINE__)

Client::Client(boost::asio::io_context& io_context) 
    : socket_(io_context),
    io_context_(io_context),
    reconnect_timer_(io_context)
{
}

void Client::Connect(const std::string& host, unsigned short port) {
    auto self = shared_from_this();

    host_ = host;
    port_ = port;

    tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host, std::to_string(port));

    boost::asio::async_connect(socket_, endpoints,
        [this, self](boost::system::error_code ec, tcp::endpoint) {
            if (!ec) {
                std::cout << "[Client] Connected to server\n";
                retry_count_ = 0;
                DoRead();
            }
            else {
                CLIENT_HANDLE_ERROR(ec);
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
        [this, self](boost::system::error_code ec, std::size_t len) {
            if (!ec) {
                std::istream is(&read_buffer_);
                std::string line = util::CleanLine(is); // 엔터등 불필요한 이스케이프 정리
                std::cout << "[Client] Received: " << line << "\n";
                DoRead();
            }
            else {
                CLIENT_HANDLE_ERROR(ec);
            }
        });
}

void Client::DoWrite() {
    auto self = shared_from_this();
    boost::asio::async_write(socket_,
        boost::asio::buffer(write_messages_.front()),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                write_messages_.pop_front();
                if (!write_messages_.empty()) {
                    DoWrite();
                }
            }
            else {
                CLIENT_HANDLE_ERROR(ec);
            }
        });
}

void Client::HandleError(const boost::system::error_code& ec,
    const char* func,
    const char* file,
    int line) {
    std::cerr << "[Client] " << func << " failed: " << ec.message()
        << " (" << file << ":" << line << ")\n";
    socket_.close();
    StartReconnect();
}

void Client::StartReconnect() {
    auto self = shared_from_this();
    std::cout << "[Client] Reconnecting in 5 seconds...\n";
    reconnect_timer_.expires_after(std::chrono::seconds(5));
    reconnect_timer_.async_wait([this, self](boost::system::error_code ec) {
        if (!ec) {
            Connect(host_, port_);
        }
        else {
            std::cerr << "Reconnect cancelled: " << ec.message() << '\n';
        }
        });
}