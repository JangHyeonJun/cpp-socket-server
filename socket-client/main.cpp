#include "Client.h"

#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	try	{
        const std::string host = (argc >= 2) ? argv[1] : "127.0.0.1";
        const unsigned short port = (argc >= 3)
            ? static_cast<unsigned short>(std::stoi(argv[2]))
            : 12345;


        boost::asio::io_context io_context;

        auto client = std::make_shared<Client>(io_context);
        client->Connect(host, port);

        std::cout << "[Main] Connecting to " << host << ":" << port << "...\n";

        // 입력 처리용 스레드 추가
        std::thread input_thread([&client]() {
            std::string line;
            while (std::getline(std::cin, line)) {
                client->Send(line);
            }
            });

        // I/O 이벤트 루프 실행
        io_context.run();

        // 입력 처리 스레드 대기
        input_thread.join();
	}
    catch (const std::exception& e) {
        std::cerr << "[Main] Exception: " << e.what() << "\n";
    }
    
    return 0;
}