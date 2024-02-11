#include "../../includes/ftIrc.hpp"

void Server::commandNICK(Client & client, Server & server)	{

	if (!client.isAuthenticated()) {
		//todo: find RPL correct numeric to send
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		server.setClientNick(client);
	}

}