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

static void commandCAP(Client & client, Server & server)
{
	(void)client;
	(void)server;
	// For now do nothing
}

static void commandPASS(Client & client, Server & server)
{
	if (client.isAuthenticated()) {
		server.sendWarning(ALREADY_AUTHENTICATED, client);
	} else {
		server.authenticate(client);
	}
}

static void commandNICK(Client & client, Server & server)
{
	if (!client.isAuthenticated()) {
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		server.setClientNick(client);
	}
}

static void commandUSER(Client & client, Server & server)
{
	if (!client.isAuthenticated()) {
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		if (client.hasUser()) {
			server.sendWarning(ALREADY_USER, client);
		} else {
			server.setClientUser(client);
		}
	}
}

static void commandJOIN(Client & client, Server & server)
{
	(void)client;
	(void)server;
	std::string s = "nickname!username@127.0.0.1 JOIN #hello";
	send(client.getFd(), s.c_str(), s.size(), MSG_NOSIGNAL);
}

//todo: find a better name for this
static int findCommandType(std::string msg)	{

	std::string commands[] = AVAILABLE_COMMANDS;

	for (int i = 0; i < NUMBER_OF_AVAILABLE_COMMANDS; i++)
	{
		if (msg.find(commands[i]) != EOS)
			return i;
	}
	return NOT_A_COMMAND;

}

void Server::parseMessage(Client & client) {

	Server::cout() << message;
	size_t end = message.find("\n");
	size_t start = 0;

	while (end != EOS) {
		std::string msg = message.substr(start, end);

		int			type = findCommandType(msg);
		if (type == NOT_A_COMMAND) {

			if (client.isRegistered()) {
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
				sendWarning(ERR_NOTREGISTERED(client.getNickname()), client);
			}

		} else {

			void	(*functions[NUMBER_OF_AVAILABLE_COMMANDS])(Client & client, Server & server) = COMMAND_FUNCTIONS;
			functions[type](client, *this);

		}

		start = end + 1;
		if (start != EOS) {
			end = message.find("\n", start);
		} else {
			end = start;
		}
	}
}