#pragma once

#include <array>
#include <memory>
#include <string>
#include <boost/asio.hpp>

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(boost::asio::ip::tcp::socket socket);

    void Start();
    void Send(std::string message);

private:
    void DoRead();
    void DoWrite();

    boost::asio::ip::tcp::socket socket_;
    std::array<char, 1024> read_buffer_{};
    std::string write_message_;
};
