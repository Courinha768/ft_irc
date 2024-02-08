#include "../../includes/Server.hpp"

void Server::authenticate(Client & client) {
	
	size_t pos;
	if (message.at(0) == '\\') {
		pos = message.find(PASS_COMMAND1);
	} else {
		pos = message.find(PASS_COMMAND2);
	}
	if(pos != EOS) {

		size_t start;
		size_t end = message.find("\n", pos);

		if (message.at(0) == '\\') {
			start = pos + 5 + 1; //this 5 is the lenght of the PASS_COMMAND1
		} else {
			start = pos + 4 + 1;
		}

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

	size_t pos = message.find(USER_COMMAND1);
	if (pos == EOS) {
		pos = message.find(USER_COMMAND2);
	}
	if (pos != EOS) {

		size_t start, end;

		if (message.at(0) == '\\') {
			start = pos + 5 + 1; //this 5 is the lenght of the USER_COMMAND1
		} else {
			start = pos + 4 + 1;
		}
		end = message.find("\n", start);

		client.setUsername(message.substr(start, end - start));
		client.setHasUser(true);
	}
}

void Server::setClientNick(Client & client) {

	size_t pos = message.find(NICK_COMMAND1);
	if (pos == EOS) {
		pos = message.find(NICK_COMMAND2);
	}
	if (pos != EOS) {
		size_t start, end;

		if (message.at(0) == '\\') {
			start = pos + 5 + 1; //this 5 is the lenght of the NICK_COMMAND1
		} else {
			start = pos + 4 + 1;
		}
		end = message.find("\n", start);

		client.setNickname(message.substr(start, end - start));
		if (!client.isRegistered()) {
			client.setisRegistered(true);
			sendRPL(client);
		}
		//todo: after the client is registered i want to create a "client id" that looks like "<client_ip>:<client_nick>"
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