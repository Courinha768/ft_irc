#include "../../includes/ftIrc.hpp"

void Server::commandPRIVMSG(Client & client)	{

	if (!client.isRegistered())	{

		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return ;

	}

	std::string trimmed_message;
	std::string	channel_name;

	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	trimmed_message = message.substr(8, end - 8);

	if (trimmed_message.empty())	{
		std::string command = "PRIVMSG";
		sendRPL(client, ERR_NEEDMOREPARAMS(command));
		return ;
	}

	end = trimmed_message.find(" ");
	channel_name = trimmed_message.substr(0, end);
	trimmed_message = trimmed_message.substr(end + 2);

	if (trimmed_message.empty())	{
		std::string command = "PRIVMSG";
		sendRPL(client, ERR_NEEDMOREPARAMS(command));
		return ;
	}

	if (!isClientOnChannel(client.getNickname(), channel_name) && !isClientOnServer(channel_name)) {
        sendRPL(client, ERR_NOTONCHANNEL(client.getNickname(), channel_name));
        return ;
    }

	if (isClientOnChannel(client.getNickname(), channel_name))	{

		for (unsigned long i = 0; i < channels.size(); i++) {

			if (channels.at(i).getName() == channel_name) {

				for (unsigned long j = 0; j < channels.at(i).getClients().size(); j++) {

					if (channels.at(i).getClients().at(j).getFd() != client.getFd()) {

						std::string test = ":" + client.getNickname() + " " + message;
						sendMsg(channels.at(i).getClients().at(j), test);

					}

				}
				break ;

			}

		}

	}	else if (isClientOnServer(channel_name))	{

		std::map<int, Client*>::iterator it = clients.begin();
		while (it != clients.end()) {
			if (it->second && it->second->getNickname().compare(channel_name) == 0) {
				std::string test = ":" + client.getNickname() + " " + message;
				sendMessageToClient(test, it->second->getFd());
				break;
			}
			it++;
		}
	}
	
}