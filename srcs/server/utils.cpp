#include "../../includes/ftIrc.hpp"

in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}

void Server::sendWarning(std::string msg, Client & client) {
	send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

// to improve!

void Server::sendRPL(Client & client) {

	std::string welcome = RPL_WELCOME(client.getNickname());
	send(client.getFd(), welcome.c_str(), welcome.size(), MSG_NOSIGNAL);

	std::string yourHost = RPL_YOURHOST(client.getNickname());
	send(client.getFd(), yourHost.c_str(), yourHost.size(), MSG_NOSIGNAL);

	std::string created = RPL_CREATED(client.getNickname());
	send(client.getFd(), created.c_str(), created.size(), MSG_NOSIGNAL);

	// include list of user modes and channels
	std::string myInfo = RPL_MYINFO(client.getNickname());
	send(client.getFd(), myInfo.c_str(), myInfo.size(), MSG_NOSIGNAL);

}