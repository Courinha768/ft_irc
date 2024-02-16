#include "../../includes/ftIrc.hpp"

void Server::commandNICK(Client & client)	{

	if (!client.isAuthenticated()) {
		//todo: find RPL correct numeric to send
		sendWarning(NEED_AUTHENTICATION, client);
	} else {

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

}