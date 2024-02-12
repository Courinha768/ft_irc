#include "../../includes/ftIrc.hpp"

typedef struct s_parsed_msg {
	std::queue<std::string>	channels;
	std::queue<std::string> passwords;
} t_parsed_msg;

static std::queue<std::pair<std::string, std::string> > parseJOINMessage(std::string message)	{

	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	message = message.substr(5, end - 5);

	std::string	channels_string;
	std::string	passwords_string;

	end = message.find(" ");
	channels_string = message.substr(0, end);
	message = message.substr(end + 1);

	end = message.find(" ");
	passwords_string = message.substr(0, end);

	std::queue<std::pair<std::string, std::string> >	commands;
	std::pair<std::string, std::string> 			pair;

	size_t coma;
	while (!channels_string.empty()) {

		coma = channels_string.find(",");
		pair.first = channels_string.substr(0, coma);
		if (coma != EOS) {
			channels_string = channels_string.substr(coma + 1);
		} else {
			channels_string = "";
		}

		coma = passwords_string.find(",");
		pair.second = passwords_string.substr(0, coma);
		if (coma != EOS) {
			passwords_string = passwords_string.substr(coma + 1);
		} else {
			passwords_string = "";
		}

		commands.push(pair);
	}

	return commands;
}

void Server::commandJOIN(Client & client)	{

	Channel	channel;

	if (client.isRegistered()) {

		std::queue<std::pair<std::string, std::string> > commands = parseJOINMessage(message);

		while (!commands.empty()) {

			bool	created = false;
			bool	wrong_pass = false;
			for (unsigned long i = 0; i < channels.size(); i++) {

				if (channels.at(i).getName() == commands.front().first) {

					channel = channels.at(i);
					if (channel.getPassword() == commands.front().second) {

						channels.at(i).addClient(client);
						created = true;

					} else {
						//!not sure this is the one
						sendRPL(client, ERR_PASSWDMISMATCH(client.getNickname()));
						wrong_pass = true;
					}

				}

			}
			if (!created && !wrong_pass) {

				channel.setName(commands.front().first);
				channel.setPassword(commands.front().second);
				channel.addClient(client);
				channels.push_back(channel);
				created = true;
				
			}
			if (!wrong_pass) {

				sendRPL(client, JOIN_REPLY(client.getNickname(), channel.getName()));
				// server.sendRPL(client, RPL_TOPIC(client.getNickname(), channel.getName(), "Wellcome to our chat room"));
				// server.sendRPL(client, RPL_NAMREPLY(client.getNickname(), channel.getName()));
				// server.sendRPL(client, RPL_ENDOFNAMES(client.getNickname(), channel.getName()));

			}

			commands.pop();

		}

	} else {

		sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));

	}


}