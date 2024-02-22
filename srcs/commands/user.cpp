#include "../../includes/ftIrc.hpp"

void Server::commandUSER(Client & client)	{

	if (!client.isAuthenticated()) {

		sendRPL(client, ERR_PASSWDMISMATCH(client.getNickname()));
		
	} else {

		if (client.isRegistered()) {
			sendRPL(client, ERR_ALREADYREGISTERED(client.getUsername()));
		} else {

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

	}

}