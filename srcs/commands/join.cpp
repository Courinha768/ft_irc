#include "../../includes/ftIrc.hpp"

void commandJOIN(Client & client, Server & server)	{

	(void)server;
	std::string hello = "hexchatClient!Hexchat@127.0.0.1 JOIN #hello\r\n";
	send(client.getFd(), hello.c_str(), hello.size(), MSG_NOSIGNAL);

	// std::string topic = RPL_TOPIC(client.getNickname(), "hello", "YES");
	// send(client.getFd(), topic.c_str(), topic.size(), MSG_NOSIGNAL);

	std::string namreply = RPL_NAMREPLY(client.getNickname(), "hello");
	send(client.getFd(), namreply.c_str(), namreply.size(), MSG_NOSIGNAL);

	std::string endofname = RPL_ENDOFNAMES(client.getNickname(), "hello");
	send(client.getFd(), endofname.c_str(), endofname.size(), MSG_NOSIGNAL);

}