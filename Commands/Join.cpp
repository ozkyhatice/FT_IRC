#include "../Server.hpp"
#include "../Channel.hpp"

void Server::join(size_t client_index)
{
    std::cout << "Input received from client " << client_index << ":" << std::endl;
    for (size_t i = 0; i < input.size(); i++)
    {
        std::cout << "input[" << i << "]: " << input[i] << std::endl;
    }
    std::cout << "Total number of inputs: " << input.size() << std::endl;


    // Check if client is connected
    if (!clients[client_index].getConnected())
    {
        clients[client_index].message(":server 451 " + clients[client_index].getNickname() + " :You have not registered\r\n");
        return;
    }

    // Check if we have enough parameters (JOIN <channel>)
    if (input.size() < 2)
    {
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " JOIN :Not enough parameters\r\n");
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " JOIN :Usage JOIN <channel> [key]\r\n");
        return;
    }

    std::string channel_name = input[1];
    std::string key;
    
    // Check if key parameter is provided (3rd parameter)
    if (input.size() == 3)
    {
        key = input[2];
    }
    else if (input.size() > 3)
    {
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " JOIN :Too many parameters\r\n");
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " JOIN :Usage JOIN <channel> [key]\r\n");
        return;
    }

    // Check if channel name starts with '#'
    if (channel_name[0] != '#')
    {
        clients[client_index].message(":server 476 " + clients[client_index].getNickname() + " " + channel_name + " :Invalid channel name\r\n");
        return;
    }

    // Check if channel already exists
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == channel_name)
        {
            // Check if client is already in the channel
            if (it->isClientInChannel(clients[client_index].getNickname()))
            {
                clients[client_index].message(":server 405 " + clients[client_index].getNickname() + " " + channel_name + " :You are already in that channel\r\n");
                return;
            }

            // Check if channel is invite-only
            if (it->getInviteOnly() && !it->isInvited(clients[client_index]))
            {
                clients[client_index].message(":server 473 " + clients[client_index].getNickname() + " " + channel_name + " :Cannot join channel (+i) - you must be invited\r\n");
                return;
            }

            // Check if channel has reached its limit
            if (it->getLimit() > 0 && it->getClients().size() >= (size_t)it->getLimit())
            {
                clients[client_index].message(":server 471 " + clients[client_index].getNickname() + " " + channel_name + " :Cannot join channel (+l) - channel is full\r\n");
                return;
            }

            // Check if channel requires a key
            if (it->getChannelKey())
            {
                if (key.empty())
                {
                    clients[client_index].message(":server 475 " + clients[client_index].getNickname() + " " + channel_name + " :Cannot join channel (+k) - key required\r\n");
                    return;
                }
                
                if (it->getKey() != key)
                {
                    clients[client_index].message(":server 475 " + clients[client_index].getNickname() + " " + channel_name + " :Cannot join channel (incorrect key)\r\n");
                    return;
                }
            }

            // Add client to channel and notify all members
            it->addClient(clients[client_index]);
            std::string join_message = ":" + clients[client_index].getNickname() + "!" + 
                                     clients[client_index].getUsername() + "@" + 
                                     clients[client_index].getIp_address() + 
                                     " JOIN " + channel_name + "\r\n";
            
            std::vector<Client> channel_clients = it->getClients();
            for (std::vector<Client>::iterator client_it = channel_clients.begin(); client_it != channel_clients.end(); ++client_it)
            {
                client_it->message(join_message);
            }

            // Send channel topic
            if (!it->getTopic().empty())
            {
                clients[client_index].message(":server 332 " + clients[client_index].getNickname() + " " + channel_name + " :" + it->getTopic() + "\r\n");
            }
            else
            {
                clients[client_index].message(":server 331 " + clients[client_index].getNickname() + " " + channel_name + " :No topic is set\r\n");
            }

            // Send names list
            std::string names_list;
            for (std::vector<Client>::iterator name_it = channel_clients.begin(); name_it != channel_clients.end(); ++name_it)
            {
                if (it->isOperator(*name_it))
                    names_list += "@";
                names_list += name_it->getNickname() + " ";
            }
            clients[client_index].message(":server 353 " + clients[client_index].getNickname() + " = " + channel_name + " :" + names_list + "\r\n");
            clients[client_index].message(":server 366 " + clients[client_index].getNickname() + " " + channel_name + " :End of /NAMES list.\r\n");
            return;
        }
    }

    // Create new channel if it doesn't exist
    Channel new_channel(channel_name, "");
    new_channel.addClient(clients[client_index]);
    new_channel.addOperator(clients[client_index]); // First user becomes operator
    
    // Set channel key if provided
    if (!key.empty())
    {
        std::cout << "Setting key for channel " << channel_name << ": " << key << std::endl;
        new_channel.setKey(key);
        new_channel.setChannelKey(true);
    }
    
    channels.push_back(new_channel);

    std::string join_message = ":" + clients[client_index].getNickname() + "!" + 
                              clients[client_index].getUsername() + "@" + 
                              clients[client_index].getIp_address() + 
                              " JOIN " + channel_name + "\r\n";
    clients[client_index].message(join_message);

    // Send default topic message for new channel
    clients[client_index].message(":server 331 " + clients[client_index].getNickname() + " " + channel_name + " :No topic is set\r\n");

    // Send names list for new channel
    clients[client_index].message(":server 353 " + clients[client_index].getNickname() + " = " + channel_name + " :@" + clients[client_index].getNickname() + "\r\n");
    clients[client_index].message(":server 366 " + clients[client_index].getNickname() + " " + channel_name + " :End of /NAMES list.\r\n");
}
