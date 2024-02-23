#include "../../includes/ftIrc.hpp"

void Server::commandINVITE(Client & client)
{
	//Again. Don't know if this is necassary. Don't believe this kind of user would get this point.
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

	std::string nickname_to_invite;
	std::string channel_name;

	size_t end = message.find("\n");
	if (end == EOS)
	{
		sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 6)), client);
		return;
	}

	if (message.at(end - 1) == '\r') end = end - 1;

	std::string to_cut = message.substr(7, end - 7);
	end = to_cut.find(" ");
	if (end == EOS)
	{
		sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 6)), client);
		return;
	}
	nickname_to_invite = to_cut.substr(0, end);
	to_cut = to_cut.substr(end + 1);

	end = to_cut.find(" ");
	if (end == EOS)
		channel_name = to_cut;
	else
		channel_name = to_cut.substr(0, end);


	Channel target_channel;

	try {
		target_channel = findChannelByName(channel_name);
	} catch (const ChannelNotFoundException& e) {
		(void)e;
		sendWarning(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name), client);
		return;
	}

	if (!isClientOnChannel(client.getNickname(), target_channel.getName()))
	{
		sendWarning(ERR_NOTONCHANNEL(client.getNickname(), channel_name), client);
		return;
	}

	Client * client_to_invite = findClientByNickname(nickname_to_invite);


	if (client_to_invite == NULL) {
		sendWarning(NOUSER, client);
		return ;
	}


	if (isClientOnChannel(client_to_invite->getNickname(), target_channel.getName()))
	{
		sendWarning(ERR_USERONCHANNEL(client.getNickname(), nickname_to_invite, channel_name), client);
		return;
	}

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels.at(i).getName().compare(channel_name) == 0) {
			channels.at(i).addInvited(*client_to_invite);
		}
	}

	sendMessageToClient(RPL_INVITING(client_to_invite->getNickname(), channel_name), client_to_invite->getFd());

}