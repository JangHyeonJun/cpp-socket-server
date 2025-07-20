#include "share.h"
#include <boost/asio.hpp>
#include <iostream>
#include <format>
#include <string>

using boost::asio::ip::tcp;


using default_token = boost::asio::as_tuple_t<boost::asio::use_awaitable_t<>>;
using tcp_acceptor = default_token::as_default_on_t<tcp::acceptor>;
using tcp_socket = default_token::as_default_on_t<tcp::socket>;

#if defined(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
# define use_awaitable \
  boost::asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

boost::asio::awaitable<void> echo(tcp_socket socket)
{
	try
	{
		char data[1024];
		for (;;)
		{
			auto [read_error, nread] = co_await socket.async_read_some(boost::asio::buffer(data));
			if (nread == 0)
				break;

			std::string response(data, nread);
			response = "\r\n response: " + response + "\r\n";
			
			auto [write_error, nwritten] = co_await async_write(socket, boost::asio::buffer(response, response.size()));
		}
	}
	catch (std::exception& e)
	{
		std::cout << std::format("echo Exception: {}\n", e.what());
	}
}

boost::asio::awaitable<void> listener(boost::asio::ip::port_type port)
{
	auto executor = co_await boost::asio::this_coro::executor;
	tcp_acceptor acceptor(executor, { tcp::v4(), port });

	for (;;)
	{
		auto [error, socket] = co_await acceptor.async_accept(); 
		if (socket.is_open())
			boost::asio::co_spawn(executor, echo(std::move(socket)), boost::asio::detached);
	}
}

uint16_t input_port(uint16_t default_port) {
	std::cout << "��Ʈ��ȣ�� �Է����ּ��� (default: " << default_port << "): ";
	std::string input;
	std::getline(std::cin, input);

	if (input.empty()) return default_port;

	int value = default_port; // �ӽ÷� int ��� (from_chars�� ��ȣ �ִ� ���� �Ľ�)
	auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), value);

	if (ec == std::errc() && value >= 0 && value <= 65535) {
		return static_cast<uint16_t>(value);
	}
	else {
		std::cout << "�߸��� �Է��Դϴ�. �⺻��(" << default_port << ") ���\n";
		return default_port;
	}
}


int main()
{
	auto port = input_port(share::DefaultPort);
	std::cout << "���õ� ��Ʈ: " << port << std::endl;

	try
	{
		boost::asio::io_context io_context(1);
		
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&](auto, auto) { io_context.stop(); });

		boost::asio::co_spawn(io_context, listener(port), boost::asio::detached);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cout << std::format("Exception: {}\n", e.what());
	}
}