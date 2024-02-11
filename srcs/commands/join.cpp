#include "../../includes/ftIrc.hpp"

typedef struct s_params {
	std::string type;
	std::string	name;
	std::string password;
} t_params;

//todo: finish this
static t_params parseJOINMessage(std::string message)	{

	t_params	params;

	size_t end = message.find("\n", 0);
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	message = message.substr(5, end); //removes the "JOIN "

	size_t first_end = message.find(" ", 0);
	params.type = message.at(0);
	if (params.type != "#" && params.type != "&") {
		// todo: error msg
	}
	params.name = message.substr(1, first_end);

	return (params);
}

void Server::commandJOIN(Client & client, Server & server)	{

	t_params	params = parseJOINMessage(message);
	bool		created = false;
	Channel		channel;

	for (unsigned long i = 0; i < channels.size(); i++) {

		if (channels.at(i).getName() == params.name) {

			channel = channels.at(i);
			channels.at(i).addClient(client);
			created = true;
			break ;

		}

	}
	if (!created) {

		channel.setName(params.name);
		channel.setPassword(params.password);
		channels.push_back(channel);
		channels.at(channels.size() - 1).addClient(client);
		
	}
		

	server.sendRPL(client, JOIN_REPLY(client.getNickname(), channel.getName()));
	// server.sendRPL(client, RPL_TOPIC(client.getNickname(), channel.getName(), "Wellcome to our chat room"));
	// server.sendRPL(client, RPL_NAMREPLY(client.getNickname(), channel.getName()));
	// server.sendRPL(client, RPL_ENDOFNAMES(client.getNickname(), channel.getName()));

}