#include "../Server.hpp"

void Server::pass(size_t client_index)
{
	if(this->clients[client_index].getConnected() == false)
	{
		if (input.size() != 2)
		{
			this->clients[client_index].message(":server 461 " + clients[client_index].getNickname() + " :Not enough parameters\r\n");
			return;
		}
		else
		{
			std::cout << "Password: " << input[1] << "-" << std::endl;
			std::cout << "serverPassword: " << this->password << "-" << std::endl;

			input[1].erase(input[1].find_last_not_of("\r") + 1);
			if (input[1] == this->password)
			{
				this->clients[client_index].setPassword(input[1]);
				this->clients[client_index].message(":server 001 " + clients[client_index].getNickname() + " :Password accepted\r\n");
			}
			else
			{
				this->clients[client_index].message(":server 464 " + clients[client_index].getNickname() + " :Password incorrect\r\n");
			}
		}
	}
	else
	{
		this->clients[client_index].message(":server 462 " + clients[client_index].getNickname() + " :You already registered\r\n");
	}
}
