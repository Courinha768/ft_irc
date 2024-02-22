#include "../../includes/ftIrc.hpp"

void Server::commandQUIT(Client & client)	{
	std::string reason = "";
	size_t pos = message.find(":");
	if (pos != EOS) {
		reason = message.substr(pos + 1);
	}
	client.setStatus(false);

	for (size_t i = 0; i < channels.size(); i++) {
		channels.at(i).removeClient(client);
	}

	// this print on server side can be ommited when project is ready
	std::cout << "connection lost with client " << client.getTextAddr() << "\n";

	std::string quit = "Quit: " + reason;
	sendMsg(client, quit);

}