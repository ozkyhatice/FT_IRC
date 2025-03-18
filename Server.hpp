#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <sstream>
#include <arpa/inet.h>

class Client;
class Channel;

class Server
{
private:
    int sockfd;
    int port;
    int max_fd;
    std::string password;
    fd_set read_fds;
    struct sockaddr_in server_addr;
    std::vector<std::string> input;

    std::vector<Client> clients;
    std::vector<Channel> channels;
    std::vector<int> connected_clients;
    typedef void (Server::*fpoint)(size_t);

public:
    Server(int port, std::string password);
    Server(Server const &server);
    Server &operator=(Server const &server);
    ~Server();

    void startServer();
    void loopProgram();
    void logControl(size_t client_index);

    void checkCommands(std::vector<char> &buffer);
    void executeCommand(size_t client_index);
    void handleCommand(size_t client_index, const std::string &command);

    bool isClientExist(std::string nickName);
    void removeClientFromChannels(size_t client_index);

    void nick(size_t client_index);
    void user(size_t client_index);
    void pass(size_t client_index);
    void privmsg(size_t client_index);
    void join(size_t client_index);
    void topic(size_t client_index);
    void kick(size_t client_index);
    void mode(size_t client_index);
    void invite(size_t client_index);
    void help(size_t client_index);
};

#endif