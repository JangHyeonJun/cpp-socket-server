#include <boost/asio.hpp>
#include <array>
#include <format>
#include <iostream>
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "client: <host>" << std::endl;
			return 1;
		}

		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);

		tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");
		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		for (;;)
		{
			std::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			if (error == boost::asio::error::eof)
				break; // Connect close by peer
			else if (error)
				throw boost::system::system_error(error); // Error

			std::cout.write(buf.data(), len);
		}

		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << std::format("Exception: {}\n", e.what());
	}
}