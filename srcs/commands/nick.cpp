#include "../../includes/ftIrc.hpp"

void Server::commandNICK(Client & client)	{

	if (!client.isAuthenticated()) {

		sendRPL(client, ERR_PASSWDMISMATCH(client.getNickname()));

	} else {


		std::string oldNick;
		std::string newNick;
		size_t pos = message.find(NICK_COMMAND);
		if (pos != EOS) {
			size_t start, end;

			start = pos + 5;
			if (start >= message.size()) {
				sendRPL(client, ERR_NONICKNAMEGIVEN(client.getUsername()));
				return;
			}
			if (!isspace(message.at(start - 1))) return;
			
			end = message.find("\n", start);
			if (isspace(message.at(end - 1))) end--; // to skip the \r when it exists
			
			newNick = message.substr(start, end - start);
			if (isMsgEmpty(newNick)) {
				sendRPL(client, ERR_NONICKNAMEGIVEN(client.getUsername()));
				return;
			}

			if (hasInvalidChars(newNick)) {
				std::string invalid_nickname = ERR_ERRONEUSNICKNAME(client.getUsername(), newNick);
				sendRPL(client, invalid_nickname);
				return;
			}

			std::map<int, Client*>::iterator it = clients.begin();
			while (it != clients.end()) {

				if (it->second && !it->second->getNickname().empty() && it->second->getNickname() == newNick) {
					sendRPL(client, ERR_NICKNAMEINUSE(client.getUsername(), newNick));
					return ;
				}
				it++;

			}
			if (client.hasNick()) oldNick = client.getNickname();
			client.setNickname(newNick);
			if (!client.isRegistered() && client.hasUser()) {

				client.setisRegistered(true);
				sendRPLwellcome(client);
				
			} else if (client.isRegistered()) {

				std::string acknowledge = ":" + oldNick + " NICK " + client.getNickname();

				std::map<int, Client*>::iterator it = clients.begin();
				while (it != clients.end()) {

					if (it->second)
						sendRPL(*it->second, acknowledge);
					it++;

				}

			}
		}

		for (unsigned long i = 0; i < channels.size(); i++)	{

			for (unsigned long j = 0; j < channels.at(i).getClients().size(); j++)	{

				if (channels.at(i).getClients().at(j).getNickname() == oldNick)	{
					channels.at(i).changeNickname(oldNick, newNick);
				}
			}
		}
	}

}