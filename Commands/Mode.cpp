#include "../Server.hpp"
#include "../Channel.hpp"

void Server::mode(size_t client_index)
{
	// Check if client is connected
	if (!clients[client_index].getConnected())
	{
		clients[client_index].message(":server 451 " + clients[client_index].getNickname() + " :You have not registered\r\n");
		return;
	}
	
	// Check if we have enough parameters (MODE <channel> <mode> <user>)
	if (input.size() < 4)
	{
		clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Not enough parameters\r\n");
		clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " MODE :Useage MODE <channel> <mode> <user>\r\n");
		return;
	}

	std::string nickname = clients[client_index].getNickname();
	std::string channel_name = input[1];
	std::string mode = input[2];
	std::string user = input[3];

	// Check if channel name starts with '#'
	if (channel_name[0] != '#')
	{
		clients[client_index].message(":server 476 " + clients[client_index].getNickname() + " " + channel_name + " :Invalid channel name\r\n");
		return;
	}

	//+o - Give operator status
	//-o - Remove operator status

	// Check if channel already exists
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getName() == channel_name)
		{
			// Check if client is not in the channel
			if (it->isClientInChannel(nickname) == false)
			{
				clients[client_index].message(":server 442 " + clients[client_index].getNickname() + " " + channel_name + " :You're not on that channel\r\n");
				return;
			}

			// Check if client is an operator
			if (it->isOperator(clients[client_index]) == false)
			{
				clients[client_index].message(":server 482 " + clients[client_index].getNickname() + " " + channel_name + " :You're not channel operator\r\n");
				return;
			}

			// Check if user is in the channel
			if (it->isClientInChannel(user) == false)
			{
				clients[client_index].message(":server 441 " + clients[client_index].getNickname() + " " + user + " " + channel_name + " :They aren't on that channel\r\n");
				return;
			}

			// // Check if user is an operator
			// if (it->isOperator(user) == true)
			// {
			// 	clients[client_index].message(":server 482 " + clients[client_index].getNickname() + " " + channel_name + " :You're not channel operator\r\n");
			// 	return;
			// }

			// Give operator status
			if (mode == "+o")
			{
				std::vector<Client> channel_clients = it->getClients();
				for (std::vector<Client>::iterator client_it = channel_clients.begin(); client_it != channel_clients.end(); ++client_it)
				{
					if (client_it->getNickname() == user)
					{
						it->addOperator(*client_it);
						break;
					}
				}
				std::string mode_message = ":" + clients[client_index].getNickname() + "!" + 
				clients[client_index].getUsername() + "@" + 
				clients[client_index].getIp_address() + 
				" MODE " + channel_name + " +o " + user + "\r\n";
				
				// Send mode message to all clients in the channel
				for (std::vector<Client>::iterator channel_client_it = channel_clients.begin(); channel_client_it != channel_clients.end(); ++channel_client_it)
				{
					channel_client_it->message(mode_message);
				}
			}
			// Remove operator status
			else if (mode == "-o")
			{
				std::vector<Client> channel_clients = it->getClients();
				std::vector<Client> channel_ops = it->getOperators();
				for (std::vector<Client>::iterator op_it = channel_ops.begin(); op_it != channel_ops.end(); ++op_it)
				{
					if (op_it->getNickname() == user)
					{
						it->removeOperator(*op_it);
						break;
					}
				}
				std::string mode_message = ":" + clients[client_index].getNickname() + "!" + 
											clients[client_index].getUsername() + "@" + 
											clients[client_index].getIp_address() + 
											" MODE " + channel_name + " -o " + user + "\r\n";
				// Send mode message to all clients in the channel
				for (std::vector<Client>::iterator channel_client_it = channel_clients.begin(); channel_client_it != channel_clients.end(); ++channel_client_it)
				{
					channel_client_it->message(mode_message);
				}
			}
			else
			{
				clients[client_index].message(":server 472 " + clients[client_index].getNickname() + " " + mode + " :Unknown MODE flag\r\n");
				return;
			}
			return;
		}
	}
}

