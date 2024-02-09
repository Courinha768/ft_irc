#include "../../includes/ftIrc.hpp"

void commandJOIN(Client & client, Server & server)	{

	//! this is just tests

	std::string confirm = ":hexchatClient!hexchatClient@ircserv.42.fr JOIN hello\r\n" ;
	server.sendRPL(client, confirm);
	server.sendRPL(client, RPL_TOPIC(client.getNickname(), "hello", "Topic"));
	server.sendRPL(client, RPL_NAMREPLY(client.getNickname(), "hello"));
	server.sendRPL(client, RPL_ENDOFNAMES(client.getNickname(), "hello"));

}