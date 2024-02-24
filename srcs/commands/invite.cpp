#include "../../includes/ftIrc.hpp"

void Server::commandINVITE(Client & client)
{
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
		sendRPL(client, ERR_NEEDMOREPARAMS(message.substr(0, 6)));
		return;
	}

	if (message.at(end - 1) == '\r') end = end - 1;

	std::string to_cut = message.substr(7, end - 7);
	end = to_cut.find(" ");
	if (end == EOS)
	{
		sendRPL(client, ERR_NEEDMOREPARAMS(message.substr(0, 6)));
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
		sendRPL(client, ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
		return;
	}

	if (!isClientOnChannel(client.getNickname(), target_channel.getName()))
	{
		sendRPL(client, ERR_NOTONCHANNEL(client.getNickname(), channel_name));
		return;
	}

	if (!client.isOperator(target_channel))	{
		sendRPL(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), target_channel.getName()));
		return ;
	}

	Client * client_to_invite = findClientByNickname(nickname_to_invite);


	if (client_to_invite == NULL) {
		sendRPL(client, NOUSER);
		return ;
	}


	if (isClientOnChannel(client_to_invite->getNickname(), target_channel.getName()))
	{
		sendRPL(client, ERR_USERONCHANNEL(client.getNickname(), nickname_to_invite, channel_name));
		return;
	}


	for (size_t i = 0; i < channels.size(); i++) {
		if (channels.at(i).getName().compare(channel_name) == 0) {
			channels.at(i).addInvited(*client_to_invite);
		}
	}


	sendRPL(*client_to_invite, RPL_INVITING(client.getNickname(), client_to_invite->getNickname(), channel_name));

}