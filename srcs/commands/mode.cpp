#include "../../includes/ftIrc.hpp"

typedef struct s_command {
	std::vector<std::string>	targets;
	std::vector<std::string>	modes;
	std::vector<std::string>	parameters;
	bool						add;
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

	end = message.find(" ");
	targets = parsed_message.substr(0, end);
	parsed_message = parsed_message.substr(end + 1);

	end = message.find(" ");
	modes = parsed_message.substr(0, end);
	parsed_message = parsed_message.substr(end + 1);

	parameters = parsed_message;

	t_command	command;
	
	end = targets.find(",");
	while (end != EOF)	{

		command.targets.push_back(targets.substr(0, end));
		targets = targets.substr(end + 1);
		end = targets.find(",");

	}
	command.targets.push_back(targets.substr(0, end));

	if (modes.at(0) == '+')
		command.add = true;
	else
		command.add = false;
	modes = modes.substr(1);

	while (!modes.empty())	{

		command.modes.push_back(modes.substr(0, 1));
		targets = targets.substr(1);

	}

	end = parameters.find(" ");
	while (end != EOF)	{

		command.parameters.push_back(parameters.substr(0, end));
		parameters = parameters.substr(end + 1);
		end = parameters.find(" ");

	}
	command.parameters.push_back(parameters.substr(0, end));

	return command;

}

void Server::commandMODE(Client & client)	{

	t_command	command = parseMODEMessage(message);

	for (unsigned long	i; i < command.targets.size(); i++)	{

		for (unsigned long	j; j < command.modes.size(); j++)	{

			for (unsigned long	k; k < command.parameters.size(); k++)	{

				

			}

		}

	}

}