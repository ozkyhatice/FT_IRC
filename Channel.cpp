#include "Channel.hpp"

Channel::Channel(std::string name, std::string topic): _name(name), _topic(topic){}

Channel::Channel(Channel const &channel): _name(channel._name), _topic(channel._topic), _clients(channel._clients), _operators(channel._operators){}

Channel &Channel::operator=(Channel const &channel){
    if (this != &channel){
        _name = channel._name;
        _topic = channel._topic;
        _clients = channel._clients;
        _operators = channel._operators;
    }
    return *this;
}

Channel::~Channel(){}

void Channel::setName(std::string name){
    _name = name;
}
std::string Channel::getName(){
    return _name;
}

void Channel::setTopic(std::string topic){
    _topic = topic;
}

std::string Channel::getTopic(){
    return _topic;
}

void Channel::addClient(Client client){
    _clients.push_back(client);
}

void Channel::removeClient(Client client){
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it){
        if (it->getNickname() == client.getNickname()){
            _clients.erase(it);
            break;
        }
    }
}

std::vector<Client> Channel::getClients(){
    return _clients;
}

void Channel::addOperator(Client client){
    _operators.push_back(client);
}
void Channel::removeOperator(Client client){
    for (std::vector<Client>::iterator it = _operators.begin(); it != _operators.end(); ++it){
        if (it->getNickname() == client.getNickname()){
            _operators.erase(it);
            break;
        }
    }
}

bool Channel::isOperator(Client client){
    for (std::vector<Client>::iterator it = _operators.begin(); it != _operators.end(); ++it){
        if (it->getNickname() == client.getNickname()){
            return true;
        }
    }
    return false;
}
std::vector<Client> Channel::getOperators(){
    return _operators;
}
