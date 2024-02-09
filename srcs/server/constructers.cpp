#include "../../includes/ftIrc.hpp"

/**
 * This function validates if the given port is a valid integer within the range of 1024 to 65535.
 */
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

/**
 * Initializes a Server with the provided port and password. If the port is valid,
 * it is assigned to the server; otherwise, the program exits with an error code.
 * The password is used to create a Password object, which is assigned to the server.
 *
 * @param port The port for the server.
 * @param password The password for the server.
 */
Server::Server(std::string port, std::string password) {

	if (isPortValid(port)) {
		this->port = port;
		eventsCount = 1;
	} else {
		exit(PORT_NOT_VALID);
	}
	this->password = new Password(password);

}

/**
 * This destructor performs cleanup operations for a Server object:
 * - Iterates through the clients map, deleting each Client object.
 * - Clears the clients map.
 * - Closes the server socket (sockfd).
 * - Frees the address information (servinfo).
 * - Deletes the Password object.
 */
Server::~Server() {

	std::map<int, Client *>::iterator it = clients.begin();
	while (it != clients.end()) {
		delete it->second;
		it++;
	}
	clients.clear();

	close(sockfd);
	freeaddrinfo(servinfo);
	delete password;

}