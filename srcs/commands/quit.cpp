#include "../../includes/ftIrc.hpp"

void Server::commandQUIT(Client & client)	{

	client.setStatus(false);
	std::cout << "connection lost with client " << client.getTextAddr() << "\n";

}