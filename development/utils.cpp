#include "../Server.hpp"

void Server::printAllInputs()
{
	std::cout << "Printing all inputs:" << std::endl;
	for (size_t i = 0; i < this->input.size(); i++)
	{
		std::cout << "input[" << i << "]: " << this->input[i] << std::endl;
	}
	std::cout << "Total number of inputs: " << this->input.size() << std::endl;
	std::cout << std::endl;
}

void Server::printAllClients()
{
	std::cout << std::endl;
	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		std::cout << "Client sockfd: " << it->getC_sockfd() << std::endl;
		std::cout << "Client Nickname: " << it->getNickname() << std::endl;
		std::cout << "Client Username: " << it->getUsername() << std::endl;
		std::cout << "Client Password: " << it->getPassword() << std::endl;
		std::cout << "Client IP Address: " << it->getIp_address() << std::endl;
		std::cout << "Client Connected: " << it->getConnected() << std::endl;
		std::cout << std::endl;
	}
}