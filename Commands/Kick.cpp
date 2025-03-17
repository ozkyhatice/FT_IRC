#include "../Server.hpp"
#include "../Channel.hpp"

void Server::kick(size_t client_index)
{
	if (!clients[client_index].getConnected())
	{
		clients[client_index].message(":server 451 " + clients[client_index].getNickname() + " :You have not registered\r\n");
		return;
	}
	if (input.size() < 3)
	{
		clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " KICK :Not enough parameters\r\n");
		clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " KICK :Usage KICK <channel> <nickname> <reason>\r\n");
		return;
	}

	std::string channel_name = input[1];
	std::string nickname = input[2];
	std::string reason = (input.size() > 3) ? input[3] : "";
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
			if (!it->isClientInChannel(nickname))
			{
				clients[client_index].message(":server 441 " + clients[client_index].getNickname() + " " + nickname + " " + channel_name + " :They aren't on that channel\r\n");
				return;
			}
			else
			{
				if (clients[client_index].getNickname() == nickname)
				{
					clients[client_index].message(":server 441 " + channel_name + " :You must select a new operator before kicking yourself.\r\n");

					std::vector<Client> channel_clients = it->getClients();
					std::string available_clients;
					for (std::vector<Client>::iterator c_it = channel_clients.begin(); c_it != channel_clients.end(); ++c_it)
					{
						if (c_it->getNickname() != clients[client_index].getNickname())
						{
							available_clients += c_it->getNickname() + " ";
						}
					}
					clients[client_index].message(":server 441 " + channel_name + " :Available clients to become new operator: " + available_clients + "\r\n");

					return;
				}
				std::vector<Client> channel_clients = it->getClients();
				for (std::vector<Client>::iterator c_it = channel_clients.begin(); c_it != channel_clients.end(); ++c_it)
				{
					if (c_it->getNickname() == nickname)
					{
						for (std::vector<Client>::iterator notify_it = channel_clients.begin(); notify_it != channel_clients.end(); ++notify_it)
						{
							notify_it->message(":" + clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@" + clients[client_index].getIp_address() + " KICK " + channel_name + " " + nickname + (reason.empty() ? "\r\n" : " :" + reason + "\r\n"));
						}
						it->removeClient(*c_it);
						c_it->message(":" + clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@" + clients[client_index].getIp_address() + " KICK " + channel_name + " " + nickname + (reason.empty() ? "\r\n" : " :" + reason + "\r\n"));

						if (it->getClients().empty())
						{
							channels.erase(it);
						}
						return;
					}
				}
			}
		}
	}

	if (!channel_found)
	{
		clients[client_index].message(":server 403 " + clients[client_index].getNickname() + " " + channel_name + " :No such channel\r\n");
	}
}