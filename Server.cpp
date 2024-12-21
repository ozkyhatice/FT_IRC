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
    while (1){
        fd_set active_fds = read_fds;
        int activity = select(max_fd + 1, &active_fds, NULL, NULL, NULL);
        if (activity < 0){
            std::cout << "Select KO" << std::endl;
            exit(1);
        }
        if (FD_ISSET(sockfd, &active_fds)){
            int client_sockfd = accept(sockfd, NULL, NULL);
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
    
    int bytes_received;
    std::vector<char> buffer(1024, 0); 
    for (size_t client_index = 0; client_index < connected_clients.size(); client_index++){
        if (FD_ISSET(connected_clients[client_index], &active_fds)){
            bytes_received = recv(connected_clients[client_index], buffer.data(), buffer.size(), 0);
            logControl(client_index);
            if (bytes_received < 0){
                close(connected_clients[client_index]);
                std::cout << "Recv KO" << std::endl;
                exit(1);
            }
            else if (bytes_received == 0){
                close(connected_clients[client_index]);
                connected_clients.erase(connected_clients.begin() + client_index);
                --client_index;
                std::cout << "Client disconnected" << std::endl;
                exit(1);
            }
            else{
                std::cout << "Received: " << buffer.data() << std::endl;
                memset(buffer.data(), 0, buffer.size());
            }
        }
    }
    }
}

void Server::logControl(size_t client_index){
    if (clients[client_index].getPassword() != "" &&
        // clients[client_index].getUsername() != "" &&
        // clients[client_index].getNickname() != "" &&
        clients[client_index].getConnected()){
            clients[client_index].setConnected(true);
            std::cout << "Client connected" << std::endl;
            clients[client_index].message(":" + clients[client_index].getIp_address() + " 001 " + clients[client_index].getNickname() + " :Welcome to the Internet Relay Network " \
						+ clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@" + clients[client_index].getIp_address() + "\r\n");
        }

}