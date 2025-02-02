#include "../Server.hpp"

void Server::printAllClients()
{
	std::cout << "input size:" << this->input.size() << std::endl;

	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		std::cout << "Client Nickname: " << it->getNickname() << std::endl;
		std::cout << "Client Username: " << it->getUsername() << std::endl;
		std::cout << "Client Password: " << it->getPassword() << std::endl;
		std::cout << "Client IP Address: " << it->getIp_address() << std::endl;
		std::cout << "Client Connected: " << it->getConnected() << std::endl;
		std::cout << std::endl;
	}
}

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
	printAllClients();
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