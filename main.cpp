#include "Server.hpp"
#include <limits>
#include <stdexcept>

Server *g_server = NULL;

void signalHandler(int code)
{
	std::cout << "Signal " << code << " caught!" << std::endl;
	if (g_server)
	{
		delete g_server;
		g_server = NULL;
	}
	exit(0);
}

bool isValidPort(const std::string &portStr)
{
	if (portStr.empty())
		return false;

	for (size_t i = 0; i < portStr.size(); i++)
	{
		if (!std::isdigit(portStr[i]))
			return false;
	}

	try
	{
		long port = std::stol(portStr);
		return port >= 0 && port <= 65535;
	}
	catch (const std::exception &)
	{
		return false;
	}
}

int main(int ac, char **av)
{
	signal(SIGINT, signalHandler);

	if (ac != 3 || !av[1] || !av[2])
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	if (!isValidPort(av[1]))
	{
		std::cerr << "Error: Port must be a number between 0 and 65535" << std::endl;
		return 1;
	}

	try
	{
		g_server = new Server(std::stoi(av[1]), av[2]);
		g_server->loopProgram();
	}
	catch (const std::exception &e)
	{
		if (g_server)
		{
			delete g_server;
			g_server = NULL;
		}
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}