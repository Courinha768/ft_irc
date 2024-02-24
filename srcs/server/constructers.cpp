#include "../../includes/ftIrc.hpp"

static bool isPortValid(std::string port) {

	if (port.find_first_not_of(NUMERALS) != EOS) {
		std::cout << "Port should be an int value" << std::endl;
		return false;
	}
	int portInt = atoi(port.c_str());
	if (portInt < RP_MIN || portInt > RP_MAX) {
		std::cout << "Port should be a number between 1024 and 65535" << std::endl;
		return false;
	}

	return true;
}

Server::Server(std::string port, std::string password) {

	if (isPortValid(port)) {
		this->port = port;
		eventsCount = 1;
	} else {
		exit(PORT_NOT_VALID);
	}
	this->password = new Password(password);

}

Server::~Server() {

	std::map<int, Client *>::iterator it = clients.begin();
	while (it != clients.end()) {
		if (it->second)
			delete it->second;
		it++;
	}
	clients.clear();

	close(sockfd);
	freeaddrinfo(servinfo);
	delete password;

}