#include "../../includes/Server.hpp"

//todo: fix problem where we can type anything in the space between the command and the input
void Server::authenticate(Client & client) {
	
	size_t pos = message.find(PASS_COMMAND);
	if(pos != EOS) {

		size_t start;
		size_t end = message.find("\n", pos);

		
		start = pos + 4 + 1;

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

//? I dont understand what is the point of the username and nickname
void Server::setClientUser(Client & client) {

	size_t pos = message.find(USER_COMMAND);
	if (pos != EOS) {

		size_t start, end;

		start = pos + 4 + 1;
		end = message.find("\n", start);

		client.setUsername(message.substr(start, end - start));
		client.setHasUser(true);
		if (!client.isRegistered() && client.hasNick()) {
			client.setisRegistered(true);
			sendRPL(client);
		}
	}
}

void Server::setClientNick(Client & client) {

	size_t pos = message.find(NICK_COMMAND);
	if (pos != EOS) {
		size_t start, end;

		start = pos + 4 + 1;
		end = message.find("\n", start);

		client.setNickname(message.substr(start, end - start));
		client.setHasNick(true);
		if (!client.isRegistered() && client.hasUser()) {
			client.setisRegistered(true);
			sendRPL(client);
		}
	}
}

// to improve!

void Server::sendRPL(Client & client) {

	std::string welcome = RPL_WELCOME(client.getNickname());
	send(client.getFd(), welcome.c_str(), welcome.size(), MSG_NOSIGNAL);

	std::string yourHost = RPL_YOURHOST(client.getNickname());
	send(client.getFd(), yourHost.c_str(), yourHost.size(), MSG_NOSIGNAL);

	std::string created = RPL_CREATED(client.getNickname());
	send(client.getFd(), created.c_str(), created.size(), MSG_NOSIGNAL);

	// include list of user modes and channels
	std::string myInfo = RPL_MYINFO(client.getNickname());
	send(client.getFd(), myInfo.c_str(), myInfo.size(), MSG_NOSIGNAL);

}