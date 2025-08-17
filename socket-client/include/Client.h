#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this<Client> {
public:
    explicit Client(boost::asio::io_context& io_context);

    void Connect(const std::string& host, unsigned short port);
    void Send(const std::string& message);
    void Close();

private:
    void DoRead();
    void DoWrite();
    void HandleError(const boost::system::error_code& ec, const char* func, const char* file, int line);
    void StartReconnect();

    tcp::socket socket_;
    boost::asio::io_context& io_context_;
    boost::asio::streambuf read_buffer_;
    std::deque<std::string> write_messages_;

    std::string host_;
    unsigned short port_;

    // 재시도 타이머
    int retry_count_ = 0;
    const int max_retries_ = 30;
    boost::asio::steady_timer reconnect_timer_;
};