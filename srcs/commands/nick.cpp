#include "../../includes/ftIrc.hpp"

void Server::commandNICK(Client & client)	{

	if (!client.isAuthenticated()) {
		//todo: find RPL correct numeric to send
		sendWarning(NEED_AUTHENTICATION, client);
	} else {
		setClientNick(client);
	}

}