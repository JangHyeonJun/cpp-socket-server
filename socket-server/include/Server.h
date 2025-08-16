#pragma once

#include <boost/asio.hpp>

class Server {
public:
    Server(boost::asio::io_context& io_context, unsigned short port);

private:
    void DoAccept();
    void DoRead();
    void DoWrite(const std::string& message);

    boost::asio::ip::tcp::acceptor acceptor_;
};
