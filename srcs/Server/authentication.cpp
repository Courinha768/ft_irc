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
			sendWarning(ERR_PASSWDMISMATCH, client);
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

		start = pos + 5;
		if (start >= message.size()) {
			sendWarning(ERR_NONICKNAMEGIVEN, client);
			return;
		}
		if (!isspace(message.at(start - 1))) return;
		
		end = message.find("\n", start);
		if (isspace(message.at(end - 1))) end--; // to skip the \r when it exists
		
		std::string newNick = message.substr(start, end - start);
		if (isMsgEmpty(newNick)) {
			sendWarning(ERR_NONICKNAMEGIVEN, client);
			return;
		}

		if (hasInvalidChars(newNick)) {
			std::string invalid_nickname = ERR_ERRONEUSNICKNAME(newNick);
			sendWarning(invalid_nickname, client);
			return;
		}

		if (client.hasNick() && client.getNickname().compare(newNick) == 0) {
			sendWarning(ERR_NICKNAMEINUSE, client);
			return ;
		}
		
		// maybe we will need a list of nicknames to store this history
		std::string oldNick;
		if (client.hasNick()) oldNick = client.getNickname();
		client.setNickname(newNick);
		client.setHasNick(true);
		if (!client.isRegistered() && client.hasUser()) {
			client.setisRegistered(true);
			sendRPL(client);
		} else if (client.isRegistered()) {
			// It should be sent to all other clients!
			std::string acknowledge = ":" + oldNick + " NICK " + client.getNickname() + "\r\n";
			sendWarning(acknowledge, client);
		}
	}
}

bool Server::isMsgEmpty(std::string msg) {
	size_t pos = 0;
	while (pos < msg.size()) {
		if (!isspace(msg.at(pos))) {
			return false;
		}
		pos++;
	}
	return true;
}

bool Server::hasInvalidChars(std::string msg) {

	if (msg.find_first_of(INVALID_LEADING_CHARS) == 0 || msg.find_first_of(WHITESPACES) != EOS) {
		return true;
	}
	return false;
	
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