#pragma once

#include <boost/asio.hpp>

class Server {
public:
    Server(boost::asio::io_context& io_context, unsigned short port);

private:
    void DoAccept();

    boost::asio::ip::tcp::acceptor acceptor_;
};
