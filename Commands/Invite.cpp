#include "../Server.hpp"
#include "../Channel.hpp"
#include "../Client.hpp"

void Server::invite(size_t client_index)
{
    if (!clients[client_index].getConnected())
    {
        clients[client_index].message(":server 451 " + clients[client_index].getNickname() + " :You have not registered\r\n");
        return;
    }
    if (input.size() < 3)
    {
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " INVITE :Not enough parameters\r\n");
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " INVITE :Usage INVITE <nickname> <channel>\r\n");
        return;
    }

    std::string target_nick = input[1];
    std::string channel_name = input[2];

    if (channel_name[0] != '#')
    {
        clients[client_index].message(":server 476 " + clients[client_index].getNickname() + " " + channel_name + " :Invalid channel name\r\n");
        return;
    }

    bool target_found = false;
    Client target_client(-1);
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->getNickname() == target_nick)
        {
            target_found = true;
            target_client = *it;
            break;
        }
    }

    if (!target_found)
    {
        clients[client_index].message(":server 401 " + clients[client_index].getNickname() + " " + target_nick + " :No such nick\r\n");
        return;
    }

    bool channel_found = false;
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == channel_name)
        {
            channel_found = true;

            if (!it->isClientInChannel(clients[client_index].getNickname()))
            {
                clients[client_index].message(":server 442 " + clients[client_index].getNickname() + " " + channel_name + " :You're not on that channel\r\n");
                return;
            }

            if (!it->isOperator(clients[client_index]))
            {
                clients[client_index].message(":server 482 " + clients[client_index].getNickname() + " " + channel_name + " :You're not channel operator\r\n");
                return;
            }
            if (it->isClientInChannel(target_nick))
            {
                clients[client_index].message(":server 443 " + clients[client_index].getNickname() + " " + target_nick + " " + channel_name + " :is already on channel\r\n");
                return;
            }
            it->addInvited(target_client);
            target_client.message(":" + clients[client_index].getNickname() + "!" +
                                  clients[client_index].getUsername() + "@" +
                                  clients[client_index].getIp_address() +
                                  " INVITE " + target_nick + " " + channel_name + "\r\n");
            clients[client_index].message(":" + clients[client_index].getIp_address() +
                                          " 341 " + clients[client_index].getNickname() +
                                          " " + target_nick + " " + channel_name + "\r\n");
            return;
        }
    }

    if (!channel_found)
    {
        clients[client_index].message(":server 403 " + clients[client_index].getNickname() + " " + channel_name + " :No such channel\r\n");
    }
}