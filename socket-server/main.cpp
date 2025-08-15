#include "Server.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

namespace
{
	unsigned short ParsePort(int argc, char* argv[],
		unsigned short default_port = 12345) {
		if (argc >= 2) {
			try {
				int parsed = std::stoi(argv[1]);
				if (parsed < 1 || parsed > 65535) {
					throw std::out_of_range("Port out of range");
				}

				return static_cast<unsigned short>(parsed);
			}
			catch (const std::exception& e) {
				std::cerr << "[Main] Invalid port argument: " << e.what()
					<< " — Using default port " << default_port << "\n";
			}
		}

		return default_port;
	}
}

int main(int argc, char* argv[])
{
	try	{
		unsigned short port = ParsePort(argc, argv);

		boost::asio::io_context io_context;
		Server server(io_context, port);

		std::cout << "[Main] Server listening on port " << port << "...\n";
		io_context.run();
	}
	catch (const std::exception& e) {
		std::cerr << "[Main] Unhandled exception: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return 0;
}