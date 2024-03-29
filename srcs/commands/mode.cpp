#include "../../includes/ftIrc.hpp"

typedef struct s_command {
	std::vector<std::string>	targets;
	std::vector<char>			modes;
	std::vector<std::string>	parameters;
} t_command;

static	t_command	parseMODEMessage(std::string message)	{

	std::string parsed_message;

	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	parsed_message = message.substr(5, end - 5);

	std::string	targets;
	std::string	modes;
	std::string	parameters;

	end = parsed_message.find(' ');
	targets = parsed_message.substr(0, end);

	if (end != EOS)	{
		
		parsed_message = parsed_message.substr(end + 1);

		end = parsed_message.find(" ");
		modes = parsed_message.substr(0, end);

		if (end != EOS)	{

			parsed_message = parsed_message.substr(end + 1);
			parameters = parsed_message;

		}

	}

	t_command	command;

	end = targets.find(",");
	while (end != EOS)	{

		command.targets.push_back(targets.substr(0, end));
		targets = targets.substr(end + 1);
		end = targets.find(",");

	}
	command.targets.push_back(targets.substr(0, end));

	if (!modes.empty())	{

		while (!modes.empty())	{
		
			command.modes.push_back((modes.substr(0, 1).c_str())[0]);
			modes = modes.substr(1);
	
		}

	}

	if (!parameters.empty())	{

		end = parameters.find(" ");
		while (end != EOS)	{

			command.parameters.push_back(parameters.substr(0, end));
			parameters = parameters.substr(end + 1);
			end = parameters.find(" ");

		}
		command.parameters.push_back(parameters.substr(0, end));

	}

	return command;

}

static std::string getModes(std::string target, std::vector<Channel> channels)	{
	
	std::string	modes = "";
	
	for (unsigned long l = 0; l < channels.size(); l++) {

		if (channels.at(l).getName() == target)	{

			if (channels.at(l).getMode()._invite_only)

				modes += "i";

			if (channels.at(l).getMode()._key)

				modes += "k";

			if (channels.at(l).getMode()._protected_topic)

				modes += "t";

			if (channels.at(l).getMode()._user_limit)

				modes += "l";

			break ;

		}			
	
	}

	if (!modes.empty())
		modes = "+" + modes;

	return modes;
}

void Server::commandMODE(Client & client)	{

	bool success = true;

	if (!client.isRegistered())	{

		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
		return ;

	}

	t_command		command = parseMODEMessage(message);
	Channel			channel;
	unsigned long	c;


	for (unsigned long	i = 1; i < command.modes.size(); i++)	{
		if (command.modes.at(i) != 'i' && command.modes.at(i) != 'k'
				&& command.modes.at(i) != 't' && command.modes.at(i) != 'l'
				&& command.modes.at(i) != 'o')	{
			sendRPL(client, ERR_UMODEUNKNOWNFLAG(client.getNickname()));
			return ;
		}
	}


	if (command.parameters.size() == 0)
		command.parameters.push_back("");
	for (unsigned long	i = 0; i < command.targets.size(); i++)	{

		if (command.modes.size() < 1)	{
			sendRPL(client, RPL_CHANNELMODEIS(client.getNickname(), command.targets.at(i), getModes(command.targets.at(i), channels)));
		} else {
			if (command.modes.at(0) != '+' && command.modes.at(0) != '-')	{
				std::string mmm = "MODE";
				std::cout << command.targets.at(i) << std::endl;
				sendRPL(client, ERR_NEEDMOREPARAMS(command.targets.at(i), client.getNickname(), mmm));
			}
		}
		for (unsigned long	j = 1; j < command.modes.size(); j++)	{

			for (unsigned long	k = 0; k < command.parameters.size(); k++)	{

				bool created = false;

				for (unsigned long l = 0; l < channels.size(); l++) {

					if (channels.at(l).getName() == command.targets.at(i)) {

						created = true;
						c = l;

					}

				}

				if (created)	{
					if (!client.isOperator(channels.at(c)))	{
						sendRPL(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channels.at(c).getName()));
					} else {

						if (command.modes.at(0) == '+')
							success =  channels.at(c).addMode(command.modes.at(j), command.parameters.at(k));
						else
							success = channels.at(c).removeMode(command.modes.at(j), command.parameters[k]);
						if (success) {
							char	symbol;
							if (command.modes.at(0) == '+')
								symbol = '+';
							else
								symbol = '-';
							if (command.parameters.at(k).empty())
								symbol = '-';
							std::string msg = ":" + client.getNickname() + " MODE " + command.targets.at(i) + " " + symbol + command.modes.at(j) + " " + command.parameters.at(k);
							for (unsigned long l = 0; l < channels.at(c).getClients().size(); l++) {

								sendRPL(channels.at(c).getClients().at(l), msg);

							}
						} else if (!success && command.modes.at(j) == 'o')	{
							if (command.parameters.at(k).empty()) {
								std::string mmm = "MODE";
								sendRPL(client, ERR_NEEDMOREPARAMS(command.targets.at(i), client.getNickname(), mmm));
							}	else if (isClientOnServer(command.parameters.at(k)))	{
								sendRPL(client, ERR_USERNOTINCHANNEL(client.getNickname(), command.parameters.at(k), channels.at(c).getName()));
							}	else	{
								sendRPL(client, NOUSER);
							}
						} else if (!success && command.modes.at(j) == 'l')	{
							std::string mmm = "MODE";
							sendRPL(client, ERR_NEEDMOREPARAMS(command.targets.at(i), client.getNickname(), mmm));;
						}

					}

				}	else	{

					sendRPL(client, ERR_NOSUCHCHANNEL(client.getNickname(), command.targets.at(i)));

				}



			}

		}

	}

}