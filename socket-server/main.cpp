#include 
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
	using namespace std::chrono;
	auto now = system_clock::now();
	return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234)

	}
	catch (std::exception& e)
	{
		std::cout << std::format("Exception: {}\n", e.what());
	}

	return 0;
}