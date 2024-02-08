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

		start = pos + 4;
		end = message.find("\n", start);
		if (isspace(message.at(end - 1))) end--;
		if (message.at(start) == ' ') {
			std::string newNick = message.substr(start + 1, end - (start + 1));

			std::cout << "newNick:" << newNick << ":"<< std::endl;
			if (isspace(newNick.at(0))) std::cout << "empty" << std::endl;
			
		
		
		// if (start >= message.size()) {
		// 	sendWarning(ERR_NONICKNAMEGIVEN, client);
		// 	return ;
		// }
		// if (message.at(start - 1) != ' ') {
		// 	std::cout << "missing space" << std::endl;
		// 	return;
		// }

		// if (client.hasNick() && client.getNickname().compare(newNick) == 0) {
		// 	sendWarning(ERR_NICKNAMEINUSE, client);
		// 	return ;
		// }
		// } else if (start >= message.size() || newNick.empty()) {
		// 	sendWarning(ERR_NONICKNAMEGIVEN, client);
		// 	return ;
		// }
		 
			client.setNickname(newNick);
			client.setHasNick(true);
			if (!client.isRegistered() && client.hasUser()) {
				client.setisRegistered(true);
				sendRPL(client);
			}
		}

		// after changing nick name, server must send a message informing the change.
		// It should be sent to all other clients!
		// <old nickname> NICK <new nickname>
		// it means we will need a list of nicknames to store this history
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