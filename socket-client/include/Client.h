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

    tcp::socket socket_;
    boost::asio::io_context& io_context_;
    boost::asio::streambuf read_buffer_;
    std::deque<std::string> write_messages_;
};