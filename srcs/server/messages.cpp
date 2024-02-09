#include "../../includes/Server.hpp"

void Server::receiveMessage(Client & client) {

	memset(recv_buffer, 0, BUFFER_SIZE);

	int bytes_recv = recv(client.getFd(), recv_buffer, BUFFER_SIZE, 0);
	//? Is this necessary
	// if (bytes_recv == -1) {

	// 	Server::cout() << BRED << "ERROR READING SOCKET" << CRESET << "\n";

	// } else 
	if (bytes_recv == 0) {

		client.setStatus(false);
		Server::cout() << "connection lost with client " << client.getTextAddr() << "\n";

	} else {

		message.append(recv_buffer);
		parseMessage(client);
		message.erase();

	}
}

void Server::sendWarning(std::string msg, Client & client) {
	send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}