#include "../Server.hpp"

void Server::pass(size_t client_index)
{
	if(this->clients[client_index].getConnected() == false)
	{
		if (input.size() != 2)
		{
			this->clients[client_index].message("461 Not enough parameters\r\n");
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
				this->clients[client_index].message("001 Password accepted\r\n");
			}
			else
			{
				this->clients[client_index].message("464 Password incorrect\r\n");
			}
		}
	}
	else
	{
		this->clients[client_index].message("462 You already registered\r\n");
	}
}
