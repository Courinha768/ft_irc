#include "../../includes/ftIrc.hpp"

void Server::commandKICK(Client &client)
{
	//We have to change by the true confirmation that the customer is an operator
	
	bool isOperator = true;

	std::string channel_name;
	std::string user_to_kick;
	std::string comment = "";


	size_t end = message.find("\n");
	// don't think we need this check. All the messages get this point with a \n in the end
	if (end == std::string::npos)
	{
		sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 4)), client);
		return;
	}

	if (message.at(end - 1) == '\r') end = end - 1;

	std::string to_cut = message.substr(5, end - 5);

	end = to_cut.find(" ");
	if (end == EOS)
	{
		sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 4)), client);
		return;
	}

	channel_name = to_cut.substr(0, end);

	try {
		Channel target_channel = findChannelByName(channel_name);
	} catch (const ChannelNotFoundException& e) {
		(void)e;
		sendWarning(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name), client);
		return;
	}


	if (channel_name.empty()) {
		sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 4)), client);
		return ;
	}

	if (!isClientOnChannel(client.getNickname(), channel_name)) {
		sendWarning(ERR_NOTONCHANNEL(client.getNickname(), channel_name), client);
		return ;
	}

	if (!isOperator)
	{
		sendWarning(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel_name), client);
		return;
	}

	// I don't know if we need this check here. Because this is checked in earlier steps. 
	// I think a not authenticated client would never get this point. Not sure!
	if (!client.isAuthenticated())
	{
		sendWarning(NEED_AUTHENTICATION, client);
		return;
	}
	//Same here!
	if (!client.isRegistered())
	{
		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return;
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
					// I think that this part is not necessary. 
					// std::string channel_notification = "Command to kick " + user_to_kick + " from " + channel_name;
					// if (!comment.empty()) {
					//     channel_notification += " using \"" + comment + "\" as the reason (comment).";
					// }
					// channel_notification += "\r\n";
					// sendMessageToAllClients(channel_notification, client.getFd());

					std::string client_notification = ":" + client.getNickname() + " kicked you from " + channel_name;
					if (!comment.empty()) {
						client_notification += " using \"" + comment + "\" as the reason (comment).";
					}
					client_notification += "\r\n";
					sendMessageToClient(client_notification, channels.at(i).getClients().at(j).getFd());

					channels.at(i).removeClient(channels.at(i).getClients().at(j));
					break;
				}
			}
		}
	}

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