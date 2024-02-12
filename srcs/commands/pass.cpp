#include "../../includes/ftIrc.hpp"

void Server::commandPASS(Client & client)	{

	if (client.isAuthenticated()) {
		sendRPL(client, ERR_ALREADYREGISTERED(client.getUsername()));
	} else {
		authenticate(client);
	}

}