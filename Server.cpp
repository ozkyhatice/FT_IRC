#include "Server.hpp"

Server::Server(int port, std::string password) : port(port), password(password) {
    startServer();
}

void Server::startServer(){
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
}