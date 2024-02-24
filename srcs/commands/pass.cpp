#include "../../includes/ftIrc.hpp"

void Server::commandPASS(Client & client)	{

	if (client.isAuthenticated()) {
		sendRPL(client, ERR_ALREADYREGISTERED(client.getUsername()));
	} else {

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
				std::string empty = "";
				sendRPL(client, ERR_NEEDMOREPARAMS(empty, client.getNickname(), command));
			}

			client.setAuthentication(password->validate(pass));

			if (!client.isAuthenticated() && pass.compare("")) {
				sendRPL(client, ERR_PASSWDMISMATCH(client.getUsername()));
			}
		}
	}

}