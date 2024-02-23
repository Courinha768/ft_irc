#include "../../includes/ftIrc.hpp"

void Server::commandPRIVMSG(Client & client)	{

	std::string trimmed_message;
	std::string	channel_name;

	size_t end = message.find("\n");
	if (message.at(end - 1) == '\r') {
		end = end - 1;
	}
	trimmed_message = message.substr(8, end - 8);

	end = trimmed_message.find(" ");
	channel_name = trimmed_message.substr(0, end);
	trimmed_message = trimmed_message.substr(end + 2);

	std::cout << HGRN << "+" << CRESET << std::endl;
	if (!isClientOnChannel(client.getNickname(), channel_name) && !isClientOnServer(channel_name)) {
        sendWarning(ERR_NOTONCHANNEL(client.getNickname(), channel_name), client);
        return ;
    }

	std::cout << HRED << "+" << CRESET << std::endl;
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

		std::cout << HBLU << "+" << CRESET << std::endl;
		for (unsigned long i = 0; i < clients.size(); i++) {

			if (clients.at(i)->getNickname() == channel_name) {

				std::string test = ":" + client.getNickname() + " " + message;
				sendMsg(*clients.at(i), test);

			}

		}

	}
	
}