#include "../../includes/ftIrc.hpp"

in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}

void Server::sendRPL(Client & client, std::string message) {
	message = message + "\r\n";
	send(client.getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
}

// to improve!

void Server::sendRPLwellcome(Client & client) {

	sendRPL(client, RPL_WELCOME(client.getNickname()));
	sendRPL(client, RPL_YOURHOST(client.getNickname()));
	sendRPL(client, RPL_CREATED(client.getNickname()));
	sendRPL(client, RPL_MYINFO(client.getNickname()));

}

bool Server::isMsgEmpty(std::string msg) {
	size_t pos = 0;
	while (pos < msg.size()) {
		if (!isspace(msg.at(pos))) {
			return false;
		}
		pos++;
	}
	return true;
}

bool Server::hasInvalidChars(std::string msg) {

	if (msg.find_first_of(INVALID_LEADING_CHARS) == 0 || msg.find_first_of(WHITESPACES) != EOS) {
		return true;
	}
	return false;
	
}

Channel & Server::findChannelByName(const std::string& name)
{
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (channels[i].getName() == name)
        {
			return channels[i];
        }
    }
    throw ChannelNotFoundException();
}

Client* Server::findClientByNickname(const std::string& nickname)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i] != NULL && clients[i]->getNickname() == nickname)
        {
            return clients[i];
        }
    }
    return NULL;
}

