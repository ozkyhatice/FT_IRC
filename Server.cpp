#include "Server.hpp"
#include "Channel.hpp"

Server::Server(int port, std::string password) : port(port), password(password)
{
    startServer();
    max_fd = sockfd;
}

Server::Server(Server const &server)
{
    port = server.port;
    password = server.password;
    sockfd = server.sockfd;
    clients = server.clients;
    connected_clients = server.connected_clients;
    read_fds = server.read_fds;
    max_fd = server.max_fd;
}

Server &Server::operator=(Server const &server)
{
    if (this != &server)
    {
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

Server::~Server()
{
    std::cout << "Server shutting down..." << std::endl;

    close(sockfd);
    for (size_t i = 0; i < connected_clients.size(); ++i)
    {
        close(connected_clients[i]);
    }
    clients.clear();
    channels.clear();
    connected_clients.clear();
}

void Server::startServer()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    server_addr.sin_addr.s_addr = INADDR_ANY;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    if (sockfd < 0)
    {
        close(sockfd);
        throw std::runtime_error("Socket creation failed");
    }
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        close(sockfd);
        throw std::runtime_error("Setsockopt failed");
    }
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close(sockfd);
        throw std::runtime_error("Bind failed");
    }
    else
        std::cout << "Bind OK" << std::endl;
    if (listen(sockfd, 3) < 0)
    {
        close(sockfd);
        throw std::runtime_error("Listen failed");
    }
    else
        std::cout << "Listen OK" << std::endl;
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
}

void Server::loopProgram()
{
    while (1)
    {
        fd_set active_fds = read_fds;
        select(max_fd + 1, &active_fds, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &active_fds))
        {
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
            if (client_sockfd < 0)
            {
                close(client_sockfd);
                std::cout << "Accept failed" << std::endl;
                continue;
            }
            else
            {
                std::cout << "Accept OK" << std::endl;
                char ip_str[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
                std::string client_ip(ip_str);

                Client new_client(client_sockfd);
                new_client.setIp_address(client_ip);
                this->clients.push_back(new_client);

                FD_SET(client_sockfd, &read_fds);
                connected_clients.push_back(client_sockfd);
                if (client_sockfd > max_fd)
                    max_fd = client_sockfd;

                new_client.message(":" + client_ip + " NOTICE * :Welcome! You can use the USER and PASS commands to authenticate.\r\n");
            }
        }

        int bytes_received;
        std::vector<char> buffer(1024, 0);
        for (size_t client_index = 0; client_index < connected_clients.size();)
        {
            if (FD_ISSET(connected_clients[client_index], &active_fds))
            {
                bytes_received = recv(connected_clients[client_index], buffer.data(), buffer.size(), 0);

                if (bytes_received < 0)
                {
                    std::cout << "Receive failed" << std::endl;
                    close(connected_clients[client_index]);
                    clients.erase(clients.begin() + client_index);
                    FD_CLR(connected_clients[client_index], &read_fds);
                    connected_clients.erase(connected_clients.begin() + client_index);
                }
                else if (bytes_received == 0)
                {
                    std::cout << clients[client_index].getNickname() << ": Client Disconnected" << std::endl;
                    removeClientFromChannels(client_index);
                    close(connected_clients[client_index]);
                    clients.erase(clients.begin() + client_index);
                    FD_CLR(connected_clients[client_index], &read_fds);
                    connected_clients.erase(connected_clients.begin() + client_index);
                }
                else if (bytes_received > 0)
                {
                    std::string received_data(buffer.data(), bytes_received);
                    clients[client_index].appendToCommandBuffer(received_data);
                    std::string &client_buffer = clients[client_index].getCommandBuffer();
                    size_t pos;
                    while ((pos = client_buffer.find("\n")) != std::string::npos)
                    {
                        std::string full_command = client_buffer.substr(0, pos);
                        if (!full_command.empty() && full_command[full_command.length() - 1] == '\r')
                            full_command = full_command.substr(0, full_command.length() - 1);
                        handleCommand(client_index, full_command);
                        client_buffer.erase(0, pos + 1);
                    }

                    memset(buffer.data(), 0, buffer.size());
                    client_index++;
                }
            }
            else
            {
                client_index++;
            }
        }
    }
}

void Server::removeClientFromChannels(size_t client_index)
{
    Client &client = clients[client_index];

    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end();)
    {
        if (it->isClientInChannel(client.getNickname()))
        {
            it->removeClient(client);
            if (it->getClients().empty())
            {
                it = channels.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void Server::logControl(size_t client_index)
{
    if (clients[client_index].getPassword() != "" &&
        clients[client_index].getUsername() != "" &&
        clients[client_index].getNickname() != "" &&
        clients[client_index].getConnected() == false)
    {
        clients[client_index].setConnected(true);
        std::cout << clients[client_index].getNickname() << ": Client Connected" << std::endl;
        clients[client_index].message(":" + clients[client_index].getIp_address() + " 001 " + clients[client_index].getNickname() + " :Welcome to the Internet Relay Network " + clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@" + clients[client_index].getIp_address() + "\r\n");
    }
}

void Server::checkCommands(std::vector<char> &buffer)
{
    std::string buffer_string(buffer.data(), buffer.size());

    while (!buffer_string.empty() && (buffer_string[buffer_string.length() - 1] == '\r' || 
                                    buffer_string[buffer_string.length() - 1] == '\n' || 
                                    buffer_string[buffer_string.length() - 1] == '\0'))
    {
        buffer_string = buffer_string.substr(0, buffer_string.length() - 1);
    }

    std::istringstream iss(buffer_string);
    std::string token;

    bool realname_started = false;
    std::string realname;

    while (iss >> token)
    {
        if (realname_started)
        {
            realname += (realname.empty() ? "" : " ") + token;
        }
        else if (token[0] == ':')
        {
            realname_started = true;
            realname = token.substr(1);
        }
        else
        {
            this->input.push_back(token);
        }
    }

    if (!realname.empty())
    {
        this->input.push_back(realname);
    }
}

void Server::executeCommand(size_t c_index)
{
    std::vector<std::string> commands;
    std::vector<fpoint> functions;

    commands.push_back("NICK");
    commands.push_back("USER");
    commands.push_back("PASS");
    commands.push_back("PRIVMSG");
    commands.push_back("JOIN");
    commands.push_back("TOPIC");
    commands.push_back("KICK");
    commands.push_back("MODE");
    commands.push_back("INVITE");
    commands.push_back("HELP");

    functions.push_back(&Server::nick);
    functions.push_back(&Server::user);
    functions.push_back(&Server::pass);
    functions.push_back(&Server::privmsg);
    functions.push_back(&Server::join);
    functions.push_back(&Server::topic);
    functions.push_back(&Server::kick);
    functions.push_back(&Server::mode);
    functions.push_back(&Server::invite);
    functions.push_back(&Server::help);

    for (size_t i = 0; i < commands.size(); i++)
    {
        for (size_t j = 0; j < this->input.size(); j++)
        {
            if (this->input[j] == commands[i])
            {
                (this->*functions[i])(c_index);
            }
        }
    }

    this->input.clear();
}

void Server::handleCommand(size_t client_index, const std::string &command)
{
    std::vector<char> command_vector(command.begin(), command.end());
    checkCommands(command_vector);
    executeCommand(client_index);
    logControl(client_index);
}

bool Server::isClientExist(std::string nickName)
{
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (nickName == it->getNickname())
            return (true);
    }
    return (false);
}
