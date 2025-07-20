#include <boost/asio.hpp>
#include <array>
#include <format>
#include <iostream>
using boost::asio::ip::tcp;


std::string get_host(int argc, char* argv[]) {
	if (argc >= 2 && argv[1] && argv[1][0] != '\0') {
		return argv[1];
	}
	std::cout << "서버 호스트를 입력하세요 (default: " << share::DefaultHost << "): ";
	std::string host;
	std::getline(std::cin, host);
	if (host.empty())
		return share::DefaultHost;
	return host;
}

unsigned short get_port(int argc, char* argv[]) {
	if (argc >= 3 && argv[2] && argv[2][0] != '\0') {
		try {
			return static_cast<unsigned short>(std::stoi(argv[2]));
		}
		catch (...) {}
	}
	std::cout << "서버 포트번호를 입력하세요 (default: " << share::DefaultPort << "): ";
	std::string port_str;
	std::getline(std::cin, port_str);
	if (port_str.empty())
		return share::DefaultPort;
	try {
		return static_cast<unsigned short>(std::stoi(port_str));
	}
	catch (...) {
		std::cout << "입력이 올바르지 않아 기본값(" << share::DefaultPort << ")을 사용합니다.\n";
		return share::DefaultPort;
	}
}

void get_time_now(int argc, char* argv[])
{
	auto host = get_host(argc, argv);
	auto port = get_port(argc, argv);

	boost::asio::io_context io_context;
	tcp::resolver resolver(io_context);

	tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
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
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[])
{
	try
	{
		for (;;)
			get_time_now(argc, argv);

		std::cout << "\n아무 키나 누르면 종료합니다...";
		std::cin.get();
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << std::format("Exception: {}\n", e.what());
	}
}