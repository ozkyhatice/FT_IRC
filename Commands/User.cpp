#include "../Server.hpp"

void Server::user(size_t client_index)
{
	if (this->clients[client_index].getConnected() == false)
	{
		if (this->input.size() == 5)
		{
			clients[client_index].setNickname(this->input[1]);
			clients[client_index].setUsername(this->input[4]);
			clients[client_index].message(":" + clients[client_index].getNickname() + " USER " + clients[client_index].getUsername() + " 0 * :" + clients[client_index].getUsername() + "\r\n");
			logControl(client_index);
		}
		else
		{
			clients[client_index].message(":" + clients[client_index].getNickname() + " 461 " + clients[client_index].getNickname() + " USER :Not enough parameters\r\n");
		}
	}
	else
	{
		clients[client_index].message(":" + clients[client_index].getNickname() + " 462 " + clients[client_index].getNickname() + " USER :You already registered\r\n");
	}
}