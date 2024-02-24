#include "../../includes/ftIrc.hpp"

void Server::commandKICK(Client &client)
{
	if (!client.isRegistered())
	{
		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return;
	}

	std::string channel_name;
	std::string user_to_kick;
	std::string comment = "";


	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') end = end - 1;

	std::string to_cut = message.substr(5, end - 5);

	end = to_cut.find(" ");
	if (end == EOS)
	{
		sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 4)), client);
		return;
	}

	channel_name = to_cut.substr(0, end);

	Channel target_channel;

	try {
		target_channel = findChannelByName(channel_name);
	} catch (const ChannelNotFoundException& e) {
		(void)e;
		sendWarning(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name), client);
		return;
	}

	if (!client.isOperator(target_channel))	{
		sendRPL(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), target_channel.getName()));
		return ;
	}

	if (channel_name.empty()) {
		sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 4)), client);
		return ;
	}

	if (!isClientOnChannel(client.getNickname(), channel_name)) {
		sendWarning(ERR_NOTONCHANNEL(client.getNickname(), channel_name), client);
		return ;
	}

	to_cut = to_cut.substr(end + 1);

	end = to_cut.find(" ");
	if (end != EOS)
	{
		user_to_kick = to_cut.substr(0, end);
		comment = to_cut.substr(end + 1);
	}
	else
		user_to_kick = to_cut;


	if (!isClientOnChannel(user_to_kick, channel_name))
	{
		sendWarning(ERR_USERNOTINCHANNEL(client.getNickname(), user_to_kick, channel_name), client);
		return;
	}

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels.at(i).getName().compare(channel_name) == 0)
		{
			for (size_t j = 0; j < channels.at(i).getClients().size(); j++)
			{
				if (channels.at(i).getClients().at(j).getNickname().compare(user_to_kick) == 0)
				{
					std::string client_notification = ":" + client.getNickname() + " KICK " + channel_name + " " + user_to_kick;
					if (!comment.empty()) {
						client_notification += " " + comment;
					}
					client_notification += "\r\n";
					//todo: msg to all clients
					sendMessageToClient(client_notification, channels.at(i).getClients().at(j).getFd());


					channels.at(i).removeClient(channels.at(i).getClients().at(j));


					if (channels.at(i).getClients().size() == 0)	{

						std::vector<Channel>::iterator it = channels.begin();
						while(it != channels.end()) {
							if ((*it).getName().compare(channel_name) == 0) {
								break;
							}
							it++;
						}
						channels.erase(it);

					}
					break;
				}
			}
		}
	}

}

bool Server::isClientOnServer(std::string client_nickname) {

	std::map<int, Client*>::iterator it = clients.begin();
	while (it != clients.end()) {
		if (it->second->getNickname().compare(client_nickname) == 0) {
			return true;
		}
		it++;
	}

	return false;
   
}

bool Server::isClientOnChannel(std::string client_nickname, std::string channel_name) {


	for (size_t i = 0; i < channels.size(); i++) {
		if (channels.at(i).getName().compare(channel_name) == 0) {
			for (size_t j = 0; j < channels.at(i).getClients().size(); j++) {
				if (channels.at(i).getClients().at(j).getNickname().compare(client_nickname) == 0) {
					return true;
				}
			}
		}
	}

	return false;
   
}