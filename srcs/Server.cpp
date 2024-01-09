#include "../includes/Server.hpp"

Server::Server(int port, std::string password) {
	this->port = port;
	this->password = password;
}

Server::~Server() {
	close(servinfo.fd);
}

void Server::initialize_server() {

	servinfo.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (servinfo.fd == -1) {
		std::cout << "ERROR" << std::endl;
	} else {
		std::cout << "OK" << std::endl;
	}
}
