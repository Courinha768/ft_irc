#include "../../includes/Server.hpp"

void commandQUIT(Client & client, Server & server)	{

	client.setStatus(false);
	server.cout() << "connection lost with client " << client.getTextAddr() << "\n";

}