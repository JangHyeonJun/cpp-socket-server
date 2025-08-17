#include "Server.h"
#include "Session.h"

#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_context& io_context, unsigned short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    DoAccept();
}

void Server::DoAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::cout << "[Server] New connection\n";
                std::make_shared<Session>(std::move(socket))->Start();
            }
            else {
                std::cout << "[Server] Accept error: " << ec.message() << "\n";
            }
            DoAccept();
        });
}
