#include "../../includes/Server.hpp"

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
		if (!client.getUsername().empty()) {
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

			void	(*functions[5])(Client & client, Server & server) = MP_COMMAND_FUNCTIONS;
			functions[type](client, *this);

		} else {

			//todo: Make this his own function
			if (client.isRegistered()) {
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
				sendWarning(ERR_NOTREGISTERED(client.getNickname()), client);
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