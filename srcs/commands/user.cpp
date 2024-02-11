#include "../../includes/ftIrc.hpp"

void Server::commandUSER(Client & client, Server & server)	{

	if (!client.isAuthenticated()) {
		//todo: find RPL correct numeric to send
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		if (!client.getUsername().empty()) {
			server.sendRPL(client, ERR_ALREADYREGISTERED(client.getUsername()));
		} else {
			server.setClientUser(client);
		}
	}

}