#include "../../includes/ftIrc.hpp"

void Server::commandTOPIC(Client & client)	{

	if (!client.isRegistered())	{

		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return ;

	}

	std::string	target;
	std::string	parameters;

	std::string parsed_message;

	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	parsed_message = message.substr(6, end - 6);

	end = parsed_message.find(' ');

	target = parsed_message.substr(0, end);


	if (end != EOS)	{
		
		parsed_message = parsed_message.substr(end + 2);
		end = parsed_message.find(" ");
		parameters = parsed_message;

	}

	Channel			channel;

	int	channel_id = -1;
	int	client_id = -1;
	int operator_id = -1;
	(void)client_id;
	(void)operator_id;

	for (unsigned long i = 0; i < channels.size(); i++) {
		if (channels.at(i).getName() == target) {
			channel_id = i;
		}
	}	if (channel_id < 0)	{
		//!Error msg
		std::cout << "Error 1" << std::endl;
		return ;
	}

	for (unsigned long i = 0; i < channels.at(channel_id).getClients().size(); i++) {
		if (client.getFd() == channels.at(channel_id).getClients().at(i).getFd())	{
			client_id = i;
		}
	}	if (channel_id < 0)	{
		//!Error msg
		std::cout << "Error 2" << std::endl;
	}

	if (parameters.empty())	{

		if (!channels.at(channel_id).getTopic().empty())
			sendRPL(client, RPL_TOPIC(client.getNickname(), channels.at(channel_id).getName(), channels.at(channel_id).getTopic()));
		else
			sendRPL(client, RPL_NOTOPIC(client.getNickname(), channels.at(channel_id).getName()));

		parameters = " ";
		return ;

	}	else	{

		for (unsigned long i = 0; i < channels.at(channel_id).getOperators().size(); i++) {
			if (client.getFd() == channels.at(channel_id).getOperators().at(i).getFd())	{
				operator_id = i;
			}
		}	if (channel_id < 0)	{
			//!Error msg
			std::cout << "Error 3" << std::endl;
			return ;
		}
		channels.at(channel_id).setTopic(parameters);
		for (unsigned long l = 0; l < channels.at(channel_id).getOperators().size(); l++)	{

			if (channels.at(channel_id).getClients().at(l).getFd() != client.getFd())	{

				if (!channels.at(channel_id).getTopic().empty())
					sendRPL(channels.at(channel_id).getClients().at(l), RPL_TOPIC(client.getNickname(), channels.at(channel_id).getName(), channels.at(channel_id).getTopic()));
				else
					sendRPL(channels.at(channel_id).getClients().at(l), RPL_NOTOPIC(client.getNickname(), channels.at(channel_id).getName()));

			}

		}

	}		

}