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

	//? is this necessary
	// sleep(2);
	
	parseMessage(client);

	message.erase();
}

void Server::parseMessage(Client & client) {

	size_t end = message.find("\n");
	size_t start = 0;

	while (end != EOS) {
		//! why is it reseting the booleans?
		//! I really dont see why its doing this and im going crazy
		//! Already checked everything and at the end of the receiveMessage func its still ok
		//! When we send a new msg and get the client again (in func setupPoll) its not there anymore
		std::string msg = message.substr(start, end);


		if (msg.find(PASS_COMMAND) != EOS) {
			if (client.isAuthenticated()) {
				send(client.getFd(), ALREADY_AUTHENTICATED, 47, 0);
			} else {
				authenticate(client);
			}
		} else if (client.isAuthenticated() && msg.find(USER_COMMAND) != EOS) {
			if (client.hasUser()) {
				send(client.getFd(), ALREADY_USER, 45, 0);
			} else {
				setClientUser(client);
			}
		} else if (client.hasUser() && msg.find(NICK_COMMAND) != EOS) {
			setClientNick(client);
		} else if (client.hasNick()) {

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
				send(client.getFd(), NEED_AUTHENTICATION, 57, 0);
			} else if (!client.hasUser()) {
				send(client.getFd(), NEED_USER, 57, 0);
			} else if (!client.hasNick()) {
				send(client.getFd(), NEED_NICK, 57, 0);
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