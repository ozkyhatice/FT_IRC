#include "Server.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <path>\n";
        return 1;
    }
    Server server(atoi(argv[1]), argv[2]);
    return 0;
}