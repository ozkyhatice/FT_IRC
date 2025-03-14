#include "../Server.hpp"

void Server::help(size_t client_index)
{
	std::string nick = this->clients[client_index].getNickname();
	
	this->clients[client_index].message(":ircserv 704 " + nick + " :HELP MENU\r\n");
	
	if (this->clients[client_index].getConnected())
	{
		this->clients[client_index].message(":ircserv 704 " + nick + " :Available Commands:\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :INVITE <nickname> <channel> - Invite a user to a channel\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :JOIN <channel> <key> - Join a channel\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :KICK <channel> <nickname> - Kick a user from a channel\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :MODE <channel/nickname> <modes> - Set channel/user modes\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :PRIVMSG <target> <message> - Send private message\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :TOPIC <channel> [topic] - View or change channel topic\r\n");
	}
	else
	{
		this->clients[client_index].message(":ircserv 704 " + nick + " :You need to login first\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :NICK <nickname>\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :USER <user> <mode> <unused> :<realname>\r\n");
		this->clients[client_index].message(":ircserv 704 " + nick + " :PASS <password>\r\n");
	}
}