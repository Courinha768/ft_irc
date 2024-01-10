#include "../includes/Server.hpp"

Server::Server(int port, std::string password) {
	this->port = port;
	this->password = password;
}

Server::~Server() {
	close(servinfo.fd);
}

void Server::initialize_server() {

	std::cout << "port: " << port << std::endl;

	servinfo.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (servinfo.fd == -1) {
		std::cout << "ERROR" << std::endl;
	} else {
		std::cout << "OK" << std::endl;
	}
}

bool Server::isPortValid(std::string port) {
	if (port.find_first_not_of("0123456789") != std::string::npos) {
		std::cout << "Port should be an int value" << std::endl;
		return false;
	}
	return true;
}
