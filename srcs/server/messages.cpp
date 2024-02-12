#include "../../includes/ftIrc.hpp"

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

static int findCommand(std::string msg)	{

	std::string commands[] = MP_AVAILABLE_COMMANDS;

	for (int i = 0; i < (int)stringVectorLenght(commands); i++)
	{
		if (msg.find(commands[i]) != EOS)
			return i;
	}
	return MP_NOT_A_COMMAND;

}

void Server::parseMessage(Client & client) {

	size_t end = message.find("\n");
	size_t start = 0;

	while (end != EOS) {

		std::string msg = message.substr(start, end);

		int	type = findCommand(msg);
		if (type != MP_NOT_A_COMMAND) {

			void	(Server::*functions[8])(Client & client) = MP_COMMAND_FUNCTIONS;
			(this->*functions[type])(client);

		} else {

			// if (!client.isRegistered()) {

			// 	sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));

			// } else {

				Server::cout() << client.getNickname() << ": " << msg << "\r\n";
			// 	for (int i = 0; i < 200; i++) {

			// 		// *Temporary
			// 		if (events[i].data.fd && events[i].data.fd != client.getFd()) {

			// 			std::stringstream ss;
			// 			ss << client.getNickname() << ": " << msg << "\r\n";
			// 			std::string message = ss.str();

			// 			send(events[i].data.fd, message.c_str(), message.size(), 0);

			// 		}

			// 	}

			// }
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