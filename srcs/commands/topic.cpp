#include "../../includes/ftIrc.hpp"

typedef struct s_command {
	std::string	target;
	std::string	parameters;
} t_command;

static	t_command	parseTOPICMessage(std::string message)	{

	std::string parsed_message;

	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	parsed_message = message.substr(6, end - 6);

	t_command	command;

	end = parsed_message.find(' ');
	command.target = parsed_message.substr(0, end);


	if (end != EOS)	{
		
		parsed_message = parsed_message.substr(end + 2);
		end = parsed_message.find(" ");

		if (end != EOS)	{
			
			command.parameters = parsed_message;

		}

	}

	return command;

}

void Server::commandTOPIC(Client & client)	{

	if (!client.isRegistered())	{

		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return ;

	}

	t_command		command = parseTOPICMessage(message);
	Channel			channel;

	std::cout << "|" << command.target << "|" << std::endl;
	std::cout << "|" << command.parameters << "|" << std::endl;


	int	channel_id = -1;
	int	client_id = -1;
	int operator_id = -1;
	(void)client_id;
	(void)operator_id;

	std::cout << HRED << "+" << CRESET << std::endl;
	for (unsigned long i = 0; i < channels.size(); i++) {
		if (channels.at(i).getName() == command.target) {
			channel_id = i;
		}
	}	if (channel_id < 0)	{
		//!Error msg
		std::cout << "Error 1" << std::endl;
		return ;
	}

	std::cout << HBLU << "+" << CRESET << std::endl;
	for (unsigned long i = 0; i < channels.at(channel_id).getClients().size(); i++) {
		if (client.getFd() == channels.at(channel_id).getClients().at(i).getFd())	{
			client_id = i;
		}
	}	if (channel_id < 0)	{
		//!Error msg
		std::cout << "Error 2" << std::endl;
	}

	std::cout << HGRN << "+" << CRESET << std::endl;
	if (command.parameters.empty())	{

		std::cout << REDB << "+" << CRESET << std::endl;
		if (!channels.at(channel_id).getTopic().empty())
			sendRPL(client, RPL_TOPIC(client.getNickname(), channels.at(channel_id).getName(), channels.at(channel_id).getTopic()));
		else
			sendRPL(client, RPL_NOTOPIC(client.getNickname(), channels.at(channel_id).getName()));

		return ;

	}	else	{

		std::cout << GRNB << "+" << CRESET << std::endl;
		for (unsigned long i = 0; i < channels.at(channel_id).getOperators().size(); i++) {
			if (client.getFd() == channels.at(channel_id).getOperators().at(i).getFd())	{
				operator_id = i;
			}
		}	if (channel_id < 0)	{
			//!Error msg
			std::cout << "Error 3" << std::endl;
			return ;
		}
		channels.at(channel_id).setTopic(command.parameters);
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