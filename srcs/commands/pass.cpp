#include "../../includes/ftIrc.hpp"

void commandPASS(Client & client, Server & server)	{

	if (client.isAuthenticated()) {
		server.sendRPL(client, ERR_ALREADYREGISTERED(client.getUsername()));
	} else {
		server.authenticate(client);
	}

}