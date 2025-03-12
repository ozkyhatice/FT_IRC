#include "../Server.hpp"

void Server::nick(size_t client_index)
{
	if (this->input.size() == 2)
	{
		if(isClientExist(this->input[1]) == false)
		{
			clients[client_index].setNickname(this->input[1]);
			clients[client_index].message(":server NICK :" + clients[client_index].getNickname() + "\r\n");
		}
		else
		{
			clients[client_index].message(":server 433 " + clients[client_index].getNickname() + " " + this->input[1] + " :Nickname is already in use\r\n");
		}
	}
	else
	{
		clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " NICK :Not enough parameters\r\n");
	}
}