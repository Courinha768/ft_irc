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
			std::string command = "PASS";
			sendRPL(client, ERR_NEEDMOREPARAMS(command));
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

		size_t start = pos + 5;
		size_t end = message.find(" ", start);

		std::string user = "~" + message.substr(start, end - start);

		if (!isspace(message.at(start - 1))) return ;

		if (start == EOS || end == EOS || (user.substr(1)).empty() || isMsgEmpty(user.substr(1))) {
			std::string command = "USER";
			sendRPL(client, ERR_NEEDMOREPARAMS(command));
			return ;
		}

		start = message.find(":");
		end = message.find("\n", start);
		if (start != EOS && end != EOS) {
			if (message.at(end - 1) == '\r') end--;
			start++;
			std::string realname = message.substr(start, end - start);
			client.setRealname(realname);
		} else return ;


		client.setUsername(user);
		if (!client.isRegistered() && !client.getNickname().empty()) {
			client.setisRegistered(true);
			sendRPLwellcome(client);
		}
	}
}

void Server::setClientNick(Client & client) {

	size_t pos = message.find(NICK_COMMAND);
	if (pos != EOS) {
		size_t start, end;

		start = pos + 5;
		if (start >= message.size()) {
			sendWarning(ERR_NONICKNAMEGIVEN(client.getUsername()), client);
			return;
		}
		if (!isspace(message.at(start - 1))) return;
		
		end = message.find("\n", start);
		if (isspace(message.at(end - 1))) end--; // to skip the \r when it exists
		
		std::string newNick = message.substr(start, end - start);
		if (isMsgEmpty(newNick)) {
			sendWarning(ERR_NONICKNAMEGIVEN(client.getUsername()), client);
			return;
		}

		if (hasInvalidChars(newNick)) {
			std::string invalid_nickname = ERR_ERRONEUSNICKNAME(client.getUsername(), newNick);
			sendWarning(invalid_nickname, client);
			return;
		}

		if (client.hasNick() && client.getNickname().compare(newNick) == 0) {
			sendWarning(ERR_NICKNAMEINUSE(client.getUsername(), newNick), client);
			return ;
		}
		
		// maybe we will need a list of nicknames to store this history
		std::string oldNick;
		if (client.hasNick()) oldNick = client.getNickname();
		client.setNickname(newNick);
		if (!client.isRegistered() && client.hasUser()) {
			client.setisRegistered(true);
			sendRPLwellcome(client);
		} else if (client.isRegistered()) {
			std::string acknowledge = ":" + oldNick + " NICK " + client.getNickname() + "\r\n";
			sendWarning(acknowledge, client);
			std::stringstream ss;
			ss << ":" << oldNick << " NICK " << client.getNickname() << "\r\n";
			std::string message = ss.str();

			sendMessageToAllClients(message, client.getFd());

		}
	}

}