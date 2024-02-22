#include "../../includes/ftIrc.hpp"

typedef struct s_command {
	std::string	target;
	std::string	parameters;
	bool		empty;
} t_command;

static	t_command	parseTOPICMessage(std::string message)	{

	std::string parsed_message;

	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	parsed_message = message.substr(6, end - 6);

	t_command	command;

	command.empty = false;

	end = parsed_message.find(' ');
	command.target = parsed_message.substr(0, end);


	if (end != EOS)	{
		
		parsed_message = parsed_message.substr(end + 2);
		end = parsed_message.find(" ");

		if (end != EOS)	{
			
			command.parameters = parsed_message;

		}

	}	else	{

		command.empty = true;

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

	for (unsigned long i = 0; i < channels.size(); i++) {

		if (channels.at(i).getName() == command.target) {
			channel = channels.at(i);

			for (unsigned long j = 0; j < channels.at(i).getClients().size(); j++) {

				if (client.getFd() == channels.at(i).getClients().at(j).getFd())	{

					if (command.empty)	{

						if (!channels.at(i).getTopic().empty())
							sendRPL(client, RPL_TOPIC(client.getNickname(), channels.at(i).getName(), channels.at(i).getTopic()));
						else
							sendRPL(client, RPL_NOTOPIC(client.getNickname(), channels.at(i).getName()));

						//!This doesnt make sense, why is it segfaulting in the return????
						std::cout << HBLU << "+" << CRESET << std::endl;
						return ;


					}	else	{
			
						for (unsigned long k = 0; k < channels.at(i).getOperators().size(); k++) {

							if (client.getFd() == channels.at(i).getOperators().at(k).getFd())	{

								channels.at(i).setTopic(command.parameters);
								for (unsigned long l = 0; l < channels.at(i).getOperators().size(); l++)	{

									if (channels.at(i).getClients().at(l).getFd() != client.getFd())	{

										if (!channels.at(i).getTopic().empty())
											sendRPL(channels.at(i).getClients().at(l), RPL_TOPIC(client.getNickname(), channels.at(i).getName(), channels.at(i).getTopic()));
										else
											sendRPL(channels.at(i).getClients().at(l), RPL_NOTOPIC(client.getNickname(), channels.at(i).getName()));

									}

								}
								return ;

							}

						}
						sendRPL(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channels.at(i).getName()));

					}
					return ;

				}

			}
			sendRPL(client, ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
			return ;

		}

	}		

}