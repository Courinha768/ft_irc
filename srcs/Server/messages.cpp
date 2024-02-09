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

	size_t end = message.find("\n");
	size_t start = 0;

	while (end != EOS) {
		std::string msg = message.substr(start, end);

		if (!isACommand(msg)) {

			if (client.isRegistered()) {
				std::cout << client.getNickname() << ": " << msg << "\r\n";

				// Sending messages to all clients connected to the server, only to test multiclients
				std::stringstream ss;
				ss << client.getNickname() << ": " << msg << "\r\n";
				std::string message = ss.str();
				sendMessageToAllClients(message, client.getFd());
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

void Server::sendMessageToAllClients(std::string msg, int client_fd) {
	for (int i = 0; i < 200; i++) {
		if (events[i].data.fd && events[i].data.fd != client_fd) {
			send(events[i].data.fd, msg.c_str(), msg.size(), 0);
		}
	}
}