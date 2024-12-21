#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>


class Server{
    private:

        struct sockaddr_in server_addr;
        int port;
        std::string password;
        int sockfd;
        std::vector<int> clients;
        fd_set read_fds;
    public:
        Server(int port, std::string password);
        Server(Server const &server);
        Server &operator=(Server const &server);
        ~Server();

        
        void startServer();



};

#endif