#include "../Server.hpp"

void Server::nick(size_t client_index)
{
	if (this->input.size() == 2)
	{
		if(isClientExist(this->input[1]) == false)
		{
			std::string oldnick = clients[client_index].getNickname();
			clients[client_index].setNickname(this->input[1]);
			clients[client_index].message(":" + oldnick + " NICK :" + this->input[1] + "\r\n");
			
			for (size_t i = 0; i < channels.size(); ++i)
			{
				Channel &channel = channels[i];
				channel.updateClientNickname(oldnick, this->input[1]);
				channel.updateOperatorNickname(oldnick, this->input[1]);
				channel.updateInvitedClientNickname(oldnick, this->input[1]);
				
				if (channel.isClientInChannel(oldnick))
				{
					std::vector<Client>& clients = const_cast<std::vector<Client>&>(channel.getClientsVector());
					for (size_t j = 0; j < clients.size(); ++j)
					{
						clients[j].message(":" + oldnick + " NICK :" + this->input[1] + "\r\n");
					}
				}
			}
		}
		else
		{
			clients[client_index].message(":server 433 * " + this->input[1] + " :Nickname is already in use\r\n");
		}
	}
	else
	{
		clients[client_index].message(":server 461 * NICK :Not enough parameters\r\n");
	}
}