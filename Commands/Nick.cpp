#include "../Server.hpp"

bool Server::isClientExist(std::string nickName)
{
	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (nickName == it->getNickname())
			return (true);
	}
	return (false);
}


void Server::nick(size_t client_index)
{
	if (this->input.size() == 2)
	{
		if(isClientExist(this->input[1]) == false)
		{
			clients[client_index].setNickname(this->input[1]);
			clients[client_index].message(":" + clients[client_index].getNickname() + " NICK :" + clients[client_index].getNickname() + "\r\n");
		}
		else
		{
			clients[client_index].message(":" + clients[client_index].getNickname() + " 433 " + clients[client_index].getNickname() + " " + this->input[1] + " :Nickname is already in use\r\n");
		}
	}
	else
	{
		clients[client_index].message(":" + clients[client_index].getNickname() + " 461 " + clients[client_index].getNickname() + " NICK :Not enough parameters\r\n");
	}
}