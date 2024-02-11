#include "../../includes/ftIrc.hpp"

typedef struct s_params {
	std::string type;
	std::string	name;
	std::string password;
	int			error;
} t_params;

enum {
	OK,
	NEED_MORE_PARAMS,


};

static std::string	removeCommand(std::string message)
{
	size_t end = message.find("\n", 0);
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	return(message.substr(5, end));
}

static t_params parseJOINMessage(std::string message)	{

	t_params	params;
	params.error = OK;

	std::string msg = removeCommand(message);
	if (msg.empty()) {
		params.error = NEED_MORE_PARAMS;
		return params;
	} else {

		if (msg.at(0) != '#' && msg.at(0) != '&') {
			// todo: error msg
		}
		
		params.type = msg.at(0);

		size_t	pos = msg.find_first_of(" ", 0);
		if (pos == EOS) {

			params.name = msg.substr(1);

		} else {

			params.name = msg.substr(1, pos);
			msg = msg.substr(pos + 1);
			if (pos == EOS) {
				params.password = msg;
			}
		}
	}

	return params;
}

//todo: enter the correct msgs to user (the first one is correct)
//todo: redo everything... see exemples in https://modern.ircdocs.horse/#join-message
void Server::commandJOIN(Client & client, Server & server)	{

	if (client.isRegistered()) {

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

	} else {

		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));

	}


}