#include "../../includes/Server.hpp"

void Server::receiveMessage(Client & client) {

	memset(recv_buffer, 0, BUFFER_SIZE);

	int bytes_recv = recv(client.getFd(), recv_buffer, BUFFER_SIZE, 0);
	if (bytes_recv == -1) {
		std::cout << BRED << "ERROR READING SOCKET" << CRESET << std::endl;
	}
	
	if (bytes_recv == 0) {
		client.setStatus(false);
		std::cout << "connection lost with client " << client.getTextAddr() << std::endl;
	}
	message.append(recv_buffer);
	sleep(2); // time to wait for the complete registration message from HexChat
	
	parseMessage(client);
	message.erase();
}

void Server::parseMessage(Client & client) {

	size_t end = message.find("\n");
	size_t start = 0;

	while (end != EOS) {
		std::string msg = message.substr(start, end);

		if (msg.find("PASS") != EOS) {
			authenticate(client);
		} else if (client.isAuthenticated() && msg.find("NICK") != EOS) {
			setClientNick(client);
		} else if (client.isAuthenticated() && msg.find("USER") != EOS) {
			setClientUser(client);
		} else if (client.isRegistered()) {
			// Print the message on the server
			std::cout << client.getNickname() << ": " << msg << "\r\n";
			// Sending messages to all clients connected to the server, only to test multiclients
			for (int i = 0; i < 200; i++) {
				if (events[i].data.fd && events[i].data.fd != client.getFd()) {
					// using stringstream to convert size_t fds to string.
					std::stringstream ss;
					ss << client.getNickname() << ": " << msg << "\r\n";
					std::string message = ss.str();
					send(events[i].data.fd, message.c_str(), message.size(), 0);
				}
			}
		} else {
			if (!client.isAuthenticated()) {
				send(client.getFd(), NEED_AUTHENTICATION, NA_SIZE, 0);
			} else if (!client.isRegistered()) {
				send(client.getFd(), NEED_REGISTRATION, NR_SIZE, 0);
			}
		}
		start = end + 1;
		if (start != EOS) end = message.find("\n", start);
		else end = start;
	}
}

void Server::sendWarning(std::string msg, Client & client) {
	send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}