#include "../../includes/ftIrc.hpp"

void Server::commandUSER(Client & client)	{

	if (!client.isAuthenticated()) {
		//todo: find RPL correct numeric to send
		sendWarning(NEED_AUTHENTICATION, client);
	} else {

		if (client.isRegistered()) {
			sendRPL(client, ERR_ALREADYREGISTERED(client.getUsername()));
		} else {
			setClientUser(client);
		}

	}

}