#include "../../includes/ftIrc.hpp"

//todo: fix problem where we can type anything in the space between the command and the input
void Server::authenticate(Client & client) {
	
	size_t pos = message.find("PASS");
	if(pos != EOS) {

		size_t start, end;
		
		start	= pos + 4 + 1;
		end		= message.find("\n", pos);
		if (message.at(end - 1) == '\r') {
			end = end - 1;
		}

		std::string pass = message.substr(start, end - start);
		client.setAuthentication(password->validate(pass));
		if (!client.isAuthenticated() && pass.compare("")) {
			sendWarning(WRONG_PASS_WARNING, client);
		} else if (!pass.compare("")) {
			sendWarning(PASS_COMMAND_USAGE, client);
		}

	}
}

void Server::setClientUser(Client & client) {

	size_t pos = message.find("USER");
	if (pos != EOS) {

		size_t start, end;

		start = pos + 4 + 1;
		// ! it was finding until the end of the string and that includes a lot more information, so i changed it to just the first space
		end = message.find(" ", start);

		std::string user = message.substr(start, end - start);
		client.setUsername(user);
		if (!client.isRegistered() && !client.getNickname().empty()) {
			client.setisRegistered(true);
			sendRPL(client);
		}
	}
}

void Server::setClientNick(Client & client) {

	size_t pos = message.find("NICK");
	if (pos != EOS) {
		size_t start, end;

		start = pos + 4 + 1;
		end = message.find("\n", start);

		std::string nick = message.substr(start, end - start);
		client.setNickname(nick);
		if (!client.isRegistered() && !client.getUsername().empty()) {
			client.setisRegistered(true);
			sendRPL(client);
		}
	}
}