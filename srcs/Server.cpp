#include "../includes/Server.hpp"

Server::Server(std::string port, std::string password) {
	if (isPortValid(port)) {
		this->port = port;
	} else {
		exit(1);
	}
	this->password = password;
	setup();
}

Server::~Server() {

	std::map<int, Client *>::iterator it = clients.begin();
	while (it != clients.end()) {
		delete it->second;
		it++;
	}
	clients.clear();

	close(sockfd);
	freeaddrinfo(servinfo);
}

bool Server::isPortValid(std::string port) {
	if (port.find_first_not_of("0123456789") != std::string::npos) {
		std::cout << "Port should be an int value" << std::endl;
		return false;
	}
	int portInt = atoi(port.c_str());
	if (portInt < 1024 || portInt > 65535) {
		std::cout << "Port should be a number between 1024 and 65535" << std::endl;
		return false;
	}
	return true;
}

void Server::setup() {
	memset(&serv, 0, sizeof(serv));

	serv.ai_family = AF_INET;
	serv.ai_socktype = SOCK_STREAM;
	serv.ai_protocol = getprotobyname("TCP")->p_proto;

	if ((status = getaddrinfo("0.0.0.0", port.c_str(), &serv, &servinfo)) != 0) {
		std::cout << "GETADDRINFO: ERROR" << std::endl;
	} else {
		std::cout << "GETADDRINFO: OK" << std::endl;
	}
}

void Server::initialize_server() {

	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd == -1) {
		std::cout << "SOCKET: ERROR" << std::endl;
	} else {
		std::cout << "SOCKET: OK" << std::endl;
	}

	if ((status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) != 0)) {
		std::cout << "BIND: ERROR" << std::endl;
	} else {
		std::cout << "BIND: OK" << std::endl;
	}

	if ((status = listen(sockfd, 5)) == -1) {
		std::cout << "LISTEN: ERROR" << std::endl;
	} else {
		std::cout << "LISTEN: OK" << std::endl;
	}

	acceptNewClient();


}

void Server::acceptNewClient() {

	struct sockaddr_storage clientAddr;
	socklen_t 				size = sizeof(clientAddr);

	int new_fd = accept(sockfd, (struct sockaddr *)&clientAddr, &size);

	if (new_fd == -1) {
		std::cout << "NEW_FD: ERROR" << std::endl;
	} else {
		clients[new_fd] = Client::createClient(clientAddr, size);
		std::cout << "NEW_FD: OK" << std::endl;
	}
	
}


