#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <cstring>



class Server{
    private:

        struct sockaddr_in server_addr;
        int port;
        std::string password;
        int sockfd;
        std::vector<Client> clients;
        std::vector<int> connected_clients;
        fd_set read_fds;
        int max_fd;
    public:
        Server(int port, std::string password);
        Server(Server const &server);
        Server &operator=(Server const &server);
        ~Server();

        
        void startServer();
        void loopProgram();
        void logControl(size_t client_index);



};

#endif