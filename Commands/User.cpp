#include "../Server.hpp"

void Server::user(size_t client_index)
{
	bool isConnected = this->clients[client_index].getConnected();
	bool isNicknameTaken = isClientExist(this->input[1]);

	if (!isConnected)
	{
		if (this->input.size() == 5)
		{
			if (isNicknameTaken)
			{
				clients[client_index].message(":server 462 " + clients[client_index].getNickname() + " :Nickname is already in use\r\n");
				return;
			}
			clients[client_index].setNickname(this->input[1]);
			clients[client_index].setUsername(this->input[4]);
			clients[client_index].message(":server USER " + clients[client_index].getNickname() + " :" + clients[client_index].getUsername() + "\r\n");
		}
		else
		{
			clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " :Not enough parameters\r\n");
		}
	}
	else
	{
		clients[client_index].message(":server 462 " + clients[client_index].getNickname() + " :You are already registered\r\n");
	}
}