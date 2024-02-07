#include "../../includes/Server.hpp"

void Server::authenticate(Client & client) {
	
	size_t pos = message.find("PASS");
		if(pos != EOS) {
			size_t end = message.find("\n", pos);
			if (message.at(end - 1) == '\r') end = end - 1;
			size_t start = pos + 5;
			std::string pass = message.substr(start, end - start); // we need to eliminate the \n on the end of the message
			client.setAuthentication(password->validate(pass));
			if (!client.isAuthenticated()) {
				sendWarning("Wrong password!\r\n", client);
			}
		}
}

void Server::setClientUser(Client & client) {
	size_t pos = message.find("USER");
	size_t start, end;
	if (pos != EOS) {
		start = pos + 5;
		end = message.find(" ", start);
		client.setUsername(message.substr(start, end - start));
	}
}

void Server::setClientNick(Client & client) {
	size_t pos = message.find("NICK");
	size_t start, end;
	if (pos != EOS) {
		start = pos + 5;
		end = message.find("\n", start);
		client.setNickname(message.substr(start, end - start));
		// std::cout << "[" << client.getNickname() << "]" << std::endl;
		//? Do we need to set nick and user for it to be registered or just nick
		client.registration(true);
		sendRPL(client);
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