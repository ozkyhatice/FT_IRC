#include "../Server.hpp"

void Server::help(size_t client_index)
{
	(void)client_index;
	std::cout << "Send Help Menu to CLient" << std::endl;
}