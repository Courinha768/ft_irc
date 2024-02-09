#include "../../includes/ftIrc.hpp"

//todo: fix problem where we can type anything in the space between the command and the input
void Server::authenticate(Client & client) {
	
	size_t pos = message.find("PASS");
	if(pos != EOS) {

		size_t start = pos + 4 + 1;
		size_t end = message.find("\n", start);
		if (message.at(end - 1) == '\r') {
			end = end - 1;
		}
		std::string pass = message.substr(start, end - start);

		if (pass.empty()) {
			sendRPL(client, ERR_NEEDMOREPARAMS(client.getUsername(), "PASS"));
		}

		client.setAuthentication(password->validate(pass));

		if (!client.isAuthenticated() && pass.compare("")) {
			sendRPL(client, ERR_PASSWDMISMATCH(client.getUsername()));
		}
	}
}

//? Should we deal with the realname??
void Server::setClientUser(Client & client) {

	size_t pos = message.find("USER");
	if (pos != EOS) {

		size_t start = pos + 4 + 1;
		size_t end = message.find(" ", start);
		if (end != EOS && message.at(end - 1) == '\r') {
			end = end - 1;
		}

		std::string user = message.substr(start, end - start);

		if (user.empty()) {
			sendRPL(client, ERR_NEEDMOREPARAMS(client.getUsername(), "USER"));
		}

		client.setUsername(user);
		if (!client.isRegistered() && !client.getNickname().empty()) {
			client.setisRegistered(true);
			sendRPLwellcome(client);
		}
	}
}

void Server::setClientNick(Client & client) {

	size_t pos = message.find("NICK");
	if (pos != EOS) {

		size_t start = pos + 4 + 1;
		size_t end = message.find("\n", start);
		if (message.at(end - 1) == '\r') {
			end = end - 1;
		}
		std::string nick = message.substr(start, end - start);

		//todo: create function that searches the server to see if the nickname already exists
		//if (nick.compare(To be done)) {
		//	sendRPL(client, ERR_NICKNAMEINUSE(client.getUsername(), nick));
		//} else 
		if (nick.empty()) {
			sendRPL(client, ERR_NONICKNAMEGIVEN(client.getUsername()));
		} else if (nick.find(" ") != EOS || nick.at(0) == '#' || nick.at(0) == ':') {
			sendRPL(client, ERR_ERRONEUSNICKNAME(client.getUsername(), nick));
		} else {

			client.setNickname(nick);
			if (!client.isRegistered() && !client.getUsername().empty()) {
				client.setisRegistered(true);
				sendRPLwellcome(client);
			}

		}

	}

}