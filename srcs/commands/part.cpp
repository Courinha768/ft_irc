#include "../../includes/ftIrc.hpp"

void Server::commandPART(Client & client)	{

	if (!client.isRegistered())
	{
		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return;
	}

	std::string channel_name;
	std::string comment = "";


	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') end = end - 1;

	std::string to_cut = message.substr(5, end - 5);

	end = to_cut.find(" ");

	channel_name = to_cut.substr(0, end);

	Channel target_channel;

	try {
		target_channel = findChannelByName(channel_name);
	} catch (const ChannelNotFoundException& e) {
		(void)e;
		sendRPL(client, ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
		return;
	}

	if (channel_name.empty()) {
		std::string empty = "";
		sendRPL(client, ERR_NEEDMOREPARAMS(empty, client.getNickname(), message.substr(0, 4)));
		return ;
	}

	if (!isClientOnChannel(client.getNickname(), channel_name)) {
		sendRPL(client, ERR_NOTONCHANNEL(client.getNickname(), channel_name));
		return ;
	}

	to_cut = to_cut.substr(end + 1);

	end = to_cut.find(" ");
	if (end != EOS)
		comment = to_cut.substr(end + 1);

	std::string msg = ":" + client.getNickname() + " " + message;

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels.at(i).getName().compare(channel_name) == 0) {
			channels.at(i).removeClient(client);
			sendMessageToClient(msg, client.getFd());
			
			std::vector<Channel>::iterator it = channels.begin();
			while (it != channels.end()) {
				if ((*it).getName().compare(channel_name) == 0) {
					if ((*it).getClients().empty()) {
						channels.erase(it);
						break ;
					} else {
						for (size_t j = 0; j < (*it).getClients().size(); j++) {
							sendMessageToClient(msg, (*it).getClients().at(j).getFd());
						}
					}
				}
				it++;
			}


			
		}
	}

	

}