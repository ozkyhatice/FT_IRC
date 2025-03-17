#include "../Server.hpp"

void Server::topic(size_t client_index)
{
    if (!clients[client_index].getConnected())
    {
        clients[client_index].message(":server 451 " + clients[client_index].getNickname() + " :You have not registered\r\n");
        return;
    }
    if (input.size() < 2)
    {
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " TOPIC :Not enough parameters\r\n");
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " TOPIC :Usage TOPIC <channel> [<topic>]\r\n");
        return;
    }

    std::string channel_name = input[1];
    std::string topic = (input.size() > 2) ? input[2] : "";
    if (channel_name[0] != '#' || channel_name.length() > 50)
    {
        clients[client_index].message(":server 476 " + clients[client_index].getNickname() + " " + channel_name + " :Invalid channel name\r\n");
        return;
    }
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == channel_name)
        {
            if (topic.empty())
            {
                clients[client_index].message(":server 332 " + clients[client_index].getNickname() + " " + channel_name + " :" + it->getTopic() + "\r\n");
                clients[client_index].message(":server 442 " + clients[client_index].getNickname() + " " + channel_name + " :You're not on that channel\r\n");
                return;
            }
            if (it->getTopicProtection() && !it->isOperator(clients[client_index]))
            {
                clients[client_index].message(":server 482 " + clients[client_index].getNickname() + " " + channel_name + " :You're not channel operator\r\n");
                return;
            }
            it->setTopic(topic);
            std::string topic_message = ":" + clients[client_index].getNickname() + "!" +
                                        clients[client_index].getUsername() + "@" +
                                        clients[client_index].getIp_address() +
                                        " TOPIC " + channel_name + " :" + topic + "\r\n";
            std::vector<Client> channel_clients = it->getClients();
            for (std::vector<Client>::iterator client_it = channel_clients.begin(); client_it != channel_clients.end(); ++client_it)
            {
                client_it->message(topic_message);
            }
            return;
        }
    }
    clients[client_index].message(":server 403 " + clients[client_index].getNickname() + " " + channel_name + " :No such channel\r\n");
}