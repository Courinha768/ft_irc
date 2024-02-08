#include "../../includes/Server.hpp"

void Server::receiveMessage(Client & client) {

	memset(recv_buffer, 0, BUFFER_SIZE);

	int bytes_recv = recv(client.getFd(), recv_buffer, BUFFER_SIZE, 0);
	if (bytes_recv == -1) {

		Server::cout() << BRED << "ERROR READING SOCKET" << CRESET << "\n";

	} else if (bytes_recv == 0) {

		client.setStatus(false);
		Server::cout() << "connection lost with client " << client.getTextAddr() << "\n";

	} else {

		message.append(recv_buffer);
		sleep(4);
		parseMessage(client);
		message.erase();
	}
}

void Server::sendWarning(std::string msg, Client & client) {
	send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

static bool isACommand(std::string msg)	{
	return (msg.find(PASS_COMMAND) != EOS || msg.find(USER_COMMAND) != EOS || msg.find(NICK_COMMAND) != EOS);
}

void Server::parseMessage(Client & client) {

	std::cout << message << "|" << std::endl;
	size_t end = message.find("\n");
	size_t start = 0;

	while (end != EOS) {
		std::string msg = message.substr(start, end);

		if (!isACommand(msg)) {

			if (client.isRegistered()) {
				std::cout << client.getNickname() << ": " << msg << "\r\n";

				// Sending messages to all clients connected to the server, only to test multiclients
				for (int i = 0; i < 200; i++) {
					if (events[i].data.fd && events[i].data.fd != client.getFd()) {
						// using stringstream to convert size_t fds to string.
						//todo: wanted to do the same we are doing to server with the server::cout() with the client if possible (just for the looks, not important)
						std::stringstream ss;
						ss << client.getNickname() << ": " << msg << "\r\n";
						std::string message = ss.str();
						send(events[i].data.fd, message.c_str(), message.size(), 0);
					}
				}
			} else {
				sendWarning(NEED_REGISTRATION, client);
			}

		} else {

			if (msg.find(PASS_COMMAND) != EOS) {

				if (client.isAuthenticated()) {
					sendWarning(ALREADY_AUTHENTICATED, client);
				} else {
					authenticate(client);
				}

			} else if (msg.find(NICK_COMMAND) != EOS) {

				if (!client.isAuthenticated()) {
					sendWarning(NEED_AUTHENTICATION, client);
				} else {
					setClientNick(client);
				}

			} else if (msg.find(USER_COMMAND) != EOS) {

				if (!client.isAuthenticated()) {
					sendWarning(NEED_AUTHENTICATION, client);
				} else {
					if (client.hasUser()) {
						sendWarning(ALREADY_USER, client);
					} else {
						setClientUser(client);
					}
				}

			} else {

				sendWarning(COMMAND_NF, client);

			}
		}

		start = end + 1;
		if (start != EOS) {
			end = message.find("\n", start);
		} else {
			end = start;
		}
	}
}