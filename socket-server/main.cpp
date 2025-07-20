#include "share.h"
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

unsigned short get_port(int argc, char* argv[]) 
{
    unsigned short port = share::DefaultPort;

    // ����� ���ڰ� ���� ���� �Ľ� �õ�
    if (argc >= 2 && argv[1] && argv[1][0] != '\0') {
        try {
            port = static_cast<unsigned short>(std::stoi(argv[1]));
            return port;
        }
        catch (...) {
            std::cout << "����� ���� ��Ʈ���� �߸��Ǿ� �����մϴ�.\n";
        }
    }

    // ����� ���ڰ� ���ų� �Ľ� ���� �� �Է� �ޱ�
    std::cout << "���� ��Ʈ��ȣ�� �Է��ϼ��� (default: " << share::DefaultPort << "): ";
    std::string port_str;
    std::getline(std::cin, port_str);
    if (port_str.empty())
        return share::DefaultPort;
    try {
        port = static_cast<unsigned short>(std::stoi(port_str));
    }
    catch (...) {
        std::cout << "�Է��� �ùٸ��� �ʾ� �⺻��(" << share::DefaultPort << ")�� ����մϴ�.\n";
        port = share::DefaultPort;
    }

    return port;
}

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
		auto port = get_port(argc, argv);
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

		for (;;)
		{
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			std::string message = make_daytime_string();

			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		}
	}
	catch (std::exception& e)
	{
		std::cout << std::format("Exception: {}\n", e.what());
	}

	std::cout << "\n�ƹ� Ű�� ������ �����մϴ�...";
	std::cin.get();
	return 0;
}