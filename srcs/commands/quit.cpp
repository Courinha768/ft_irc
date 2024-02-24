#include "../../includes/ftIrc.hpp"

void Server::commandQUIT(Client & client)	{

	if (!client.isRegistered())	{

		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return ;

	}
	
	std::string reason = "";
	size_t pos = message.find(":");
	if (pos != EOS) {
		reason = message.substr(pos + 1);
	}
	client.setStatus(false);
	std::cout << "connection lost with client " << client.getNickname() << "\n";

	std::vector<std::vector<Channel>::iterator> list_of_empty_channels;

	std::string msg = ":" + client.getNickname() + " ERROR :" + reason + "\r\n"; 
	std::vector<Channel>::iterator it = channels.begin();
	while (it != channels.end()) {
		sendMessageToClient(msg, client.getFd());
		(*it).removeClient(client);
		if ((*it).getClients().empty()) {
			list_of_empty_channels.push_back(it);
		} else {
			std::string part_msg = ":" + client.getNickname() + " PART " + it->getName() + " :" + reason + "\r\n"; 
			for (size_t j = 0; j < (*it).getClients().size(); j++) {
				sendMessageToClient(part_msg, (*it).getClients().at(j).getFd());
			}
		}
		it++;
	}

	for (size_t i = 0; i < list_of_empty_channels.size(); i++) {
		channels.erase(list_of_empty_channels.at(i));
	}


}