#include "../../includes/Server.hpp"

static void commandCAP(Client & client, Server & server)	{

	(void)client;
	(void)server;

}

static void commandPASS(Client & client, Server & server)	{

	if (client.isAuthenticated()) {
		server.sendWarning(ALREADY_AUTHENTICATED, client);
	} else {
		server.authenticate(client);
	}

}

static void commandNICK(Client & client, Server & server)	{

	if (!client.isAuthenticated()) {
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		server.setClientNick(client);
	}

}

static void commandUSER(Client & client, Server & server)	{

	if (!client.isAuthenticated()) {
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		if (!client.getUsername().empty()) {
			server.sendWarning(ALREADY_USER, client);
		} else {
			server.setClientUser(client);
		}
	}

}

static void commandJOIN(Client & client, Server & server)	{

	(void)client;
	(void)server;

}

static void commandQUIT(Client & client, Server & server)	{

	client.setStatus(false);
	server.cout() << "connection lost with client " << client.getTextAddr() << "\n";

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

	Server::cout() << message;

	size_t end = message.find("\n");
	size_t start = 0;

	while (end != EOS) {

		std::string msg = message.substr(start, end);

		int	type = findCommand(msg);
		if (type != MP_NOT_A_COMMAND) {

			void	(*functions[6])(Client & client, Server & server) = MP_COMMAND_FUNCTIONS;
			functions[type](client, *this);

		} else {

			if (!client.isRegistered()) {

				sendWarning(ERR_NOTREGISTERED(client.getNickname()), client);

			} else {

				for (int i = 0; i < 200; i++) {

					if (events[i].data.fd && events[i].data.fd != client.getFd()) {

						std::stringstream ss;
						ss << client.getNickname() << ": " << msg << "\r\n";
						std::string message = ss.str();

						send(events[i].data.fd, message.c_str(), message.size(), 0);

					}

				}

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