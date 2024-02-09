#include "../../includes/ftIrc.hpp"

void commandPASS(Client & client, Server & server)	{

	if (client.isAuthenticated()) {
		server.sendWarning(ALREADY_AUTHENTICATED, client);
	} else {
		server.authenticate(client);
	}

}