#include "../../includes/ftIrc.hpp"

void commandUSER(Client & client, Server & server)	{

	if (!client.isAuthenticated()) {
		server.sendWarning(NEED_AUTHENTICATION, client);
	} else {
		if (!client.getUsername().empty()) {
			server.sendWarning(ALREADY_USER, client);
		} else {
			server.setClientUser(client);
		}
	}

}