#include "../../includes/ftIrc.hpp"

void commandNICK(Client & client, Server & server)	{

	if (!client.isAuthenticated()) {
		//todo: find RPL correct numeric to send
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		server.setClientNick(client);
	}

}