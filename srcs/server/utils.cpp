#include "../../includes/ftIrc.hpp"

in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}

void Server::sendWarning(std::string msg, Client & client) {
	send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}


void Server::sendRPL(Client & client, std::string message) {
	message = message + "\r\n";
	send(client.getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
}

// to improve!

void Server::sendRPLwellcome(Client & client) {

	sendRPL(client, RPL_WELCOME(client.getNickname()));
	sendRPL(client, RPL_YOURHOST(client.getNickname()));
	sendRPL(client, RPL_CREATED(client.getNickname()));
	sendRPL(client, RPL_MYINFO(client.getNickname()));

}