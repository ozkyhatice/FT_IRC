#include "../Server.hpp"
#include "../Channel.hpp"

void Server::mode(size_t client_index)
{
    if (!clients[client_index].getConnected())
    {
        clients[client_index].message(":server 451 " + clients[client_index].getNickname() + " :You have not registered\r\n");
        return;
    }
    if (input.size() < 3)
    {
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Not enough parameters\r\n");
        clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Usage MODE <channel> <mode> [parameter]\r\n");
        return;
    }

    std::string nickname = clients[client_index].getNickname();
    std::string channel_name = input[1];
    std::string mode = input[2];
    std::string parameter = "";
    if (input.size() > 3)
    {
        parameter = input[3];
    }
    if (channel_name[0] != '#')
    {
        clients[client_index].message(":server 476 " + clients[client_index].getNickname() + " " + channel_name + " :Invalid channel name\r\n");
        return;
    }
    bool channel_found = false;
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == channel_name)
        {
            channel_found = true;
            if (!it->isClientInChannel(nickname))
            {
                clients[client_index].message(":server 442 " + clients[client_index].getNickname() + " " + channel_name + " :You're not on that channel\r\n");
                return;
            }

            if (!it->isOperator(clients[client_index]))
            {
                clients[client_index].message(":server 482 " + clients[client_index].getNickname() + " " + channel_name + " :You're not channel operator\r\n");
                return;
            }

            std::vector<Client> channel_clients = it->getClients();
            std::string mode_message;
            if (mode == "+o")
            {
                if (parameter.empty())
                {
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Not enough parameters\r\n");
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Usage MODE <channel> +o <nickname>\r\n");
                    return;
                }

                if (!it->isClientInChannel(parameter))
                {
                    clients[client_index].message(":server 441 " + clients[client_index].getNickname() + " " + parameter + " " + channel_name + " :They aren't on that channel\r\n");
                    return;
                }

                for (std::vector<Client>::iterator client_it = channel_clients.begin(); client_it != channel_clients.end(); ++client_it)
                {
                    if (client_it->getNickname() == parameter)
                    {
                        it->addOperator(*client_it);
                        break;
                    }
                }
                mode_message = ":" + nickname + " MODE " + channel_name + " +o " + parameter + "\r\n";
            }
            else if (mode == "-o")
            {
                if (parameter.empty())
                {
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Not enough parameters\r\n");
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Usage MODE <channel> -o <nickname>\r\n");
                    return;
                }
                if (!it->isClientInChannel(parameter))
                {
                    clients[client_index].message(":server 441 " + clients[client_index].getNickname() + " " + parameter + " " + channel_name + " :They aren't on that channel\r\n");
                    return;
                }
                std::vector<Client> channel_ops = it->getOperators();
                for (std::vector<Client>::iterator op_it = channel_ops.begin(); op_it != channel_ops.end(); ++op_it)
                {
                    if (op_it->getNickname() == parameter)
                    {
                        it->removeOperator(*op_it);
                        break;
                    }
                }
                mode_message = ":" + nickname + " MODE " + channel_name + " -o " + parameter + "\r\n";
            }
            else if (mode == "+k")
            {
                if (parameter.empty())
                {
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Not enough parameters\r\n");
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Usage MODE <channel> +k <key>\r\n");
                    return;
                }

                it->setKey(parameter);
                it->setChannelKey(true);
                mode_message = ":" + nickname + " MODE " + channel_name + " +k " + parameter + "\r\n";
            }
            else if (mode == "-k")
            {
                it->setKey("");
                it->setChannelKey(false);
                mode_message = ":" + nickname + " MODE " + channel_name + " -k\r\n";
            }
            else if (mode == "+i")
            {
                it->setInviteOnly(true);
                mode_message = ":" + nickname + " MODE " + channel_name + " +i\r\n";
            }
            else if (mode == "-i")
            {
                it->setInviteOnly(false);
                mode_message = ":" + nickname + " MODE " + channel_name + " -i\r\n";
            }
            else if (mode == "+t")
            {
                it->setTopicProtection(true);
                mode_message = ":" + nickname + " MODE " + channel_name + " +t\r\n";
            }
            else if (mode == "-t")
            {
                it->setTopicProtection(false);
                mode_message = ":" + nickname + " MODE " + channel_name + " -t\r\n";
            }
            else if (mode == "+l")
            {
                if (parameter.empty())
                {
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Not enough parameters\r\n");
                    clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Usage MODE <channel> +l <limit>\r\n");
                    return;
                }
                size_t new_limit = std::atoi(parameter.c_str());
            if (new_limit < it->getClients().size())
            {
                clients[client_index].message(":server 472 " + clients[client_index].getNickname() + " " + channel_name + " :Cannot set limit lower than the current number of users in the channel\r\n");
                return;
            }

            it->setLimit(new_limit);
            mode_message = ":" + nickname + " MODE " + channel_name + " +l " + parameter + "\r\n";
            }
            else if (mode == "-l")
            {
                it->setLimit(0);
                mode_message = ":" + nickname + " MODE " + channel_name + " -l\r\n";
            }
            else
            {
                clients[client_index].message(":server 472 " + clients[client_index].getNickname() + " " + mode + " :Unknown MODE flag\r\n");
                return;
            }
            for (std::vector<Client>::iterator client_it = channel_clients.begin(); client_it != channel_clients.end(); ++client_it)
            {
                client_it->message(mode_message);
            }
            return;
        }
    }
    
    if (!channel_found)
    {
        clients[client_index].message(":server 403 " + clients[client_index].getNickname() + " " + channel_name + " :No such channel\r\n");
    }
}
