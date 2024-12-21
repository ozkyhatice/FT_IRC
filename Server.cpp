#include "Server.hpp"


Server::Server(int port, std::string password) : port(port), password(password) {
    startServer();
    max_fd = sockfd;
    loopProgram();
}

Server::Server(Server const &server){
    port = server.port;
    password = server.password;
    sockfd = server.sockfd;
    clients = server.clients;
    connected_clients = server.connected_clients;
    read_fds = server.read_fds;
    max_fd = server.max_fd;
}

Server &Server::operator=(Server const &server){
    if (this != &server){
        port = server.port;
        password = server.password;
        sockfd = server.sockfd;
        clients = server.clients;
        connected_clients = server.connected_clients;
        read_fds = server.read_fds;
        max_fd = server.max_fd;
    }
    return *this;
}

Server::~Server(){}

void Server::startServer(){
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        close(sockfd);
        std::cout << "Socket KO" << std::endl;
        exit(1);
    }
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        close(sockfd);
        std::cout << "Setsockopt KO" << std::endl;
        exit(1);
    }
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        close(sockfd);
        std::cout << "Bind KO" << std::endl;
        exit(1);
    }
    else
        std::cout << "Bind OK" << std::endl;
    if (listen(sockfd, 3) < 0){
        close(sockfd);
        std::cout << "Listen KO" << std::endl;
        exit(1);
    }
    else
        std::cout << "Listen OK" << std::endl;
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
}

void Server::loopProgram(){
    int client_sockfd;
    while (1){
        fd_set active_fds = read_fds;
        int activity = select(max_fd + 1, &active_fds, NULL, NULL, NULL);
        if (activity < 0){
            std::cout << "Select KO" << std::endl;
            exit(1);
        }
        if (FD_ISSET(sockfd, &active_fds)){
            client_sockfd = accept(sockfd, NULL, NULL);
            if (client_sockfd < 0){
                close(client_sockfd);
                std::cout << "Accept KO" << std::endl;
                exit(1);
            }
            else{
                std::cout << "Accept OK" << std::endl;
                FD_SET(client_sockfd, &read_fds);
                connected_clients.push_back(client_sockfd);
                if (client_sockfd > max_fd)
                    max_fd = client_sockfd;
            }
            Client new_client(client_sockfd);
            this->clients.push_back(new_client);
        }
    }
}