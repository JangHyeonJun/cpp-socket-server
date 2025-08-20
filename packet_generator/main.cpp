#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main() {
    const std::string protoc_path =
        "C:\\vcpkg\\installed\\x64-windows\\tools\\protobuf\\protoc.exe";
    const std::string grpc_plugin_path =
        "C:\\vcpkg\\installed\\x64-windows\\tools\\grpc\\grpc_cpp_plugin.exe";

    const std::string scehma_path = ".\\proto\\echo.proto";
    const std::string output_path = "..\\generated";

    fs::create_directories(output_path);

    const std::string cmd1 = "\"" + protoc_path + "\" -I=proto --cpp_out=" + output_path + " " + scehma_path;
    const std::string cmd2 =
        "\"" + protoc_path + "\" -I=proto --grpc_out=" + output_path +
        " --plugin=protoc-gen-grpc=" + grpc_plugin_path + " " + scehma_path;

    std::cout << "[packet_generator] " << cmd1 << "\n";
    if (std::system(cmd1.c_str()) != 0) return 1;

    std::cout << "[packet_generator] " << cmd2 << "\n";
    if (std::system(cmd2.c_str()) != 0) return 1;

    std::cout << "[packet_generator] done\n";
    return 0;
}
