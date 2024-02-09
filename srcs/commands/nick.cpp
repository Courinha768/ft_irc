#include "../../includes/ftIrc.hpp"

void commandNICK(Client & client, Server & server)	{

	if (!client.isAuthenticated()) {
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		server.setClientNick(client);
	}

}