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
	monitored_fds.push_back(sockfd);
	if (sockfd == -1) {
		std::cout << "SOCKET: ERROR" << std::endl;
	} else {
		std::cout << "SOCKET: OK" << std::endl;
	}
	int yes = 1;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		std::cout << "set socket error" << std::endl;
	} else {
		std::cout << "set socket ok" << std::endl;
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

	while (1) {
		acceptNewClient();
	}

}

in_addr Server::get_in_addr(struct sockaddr *sa){

	return (((struct sockaddr_in*)sa)->sin_addr);

}

void Server::setFds(fd_set *ptr) {
	FD_ZERO(ptr);

	std::vector<int>::iterator it = monitored_fds.begin();

	while (it != monitored_fds.end()) {
		FD_SET(*it, ptr);
		it++;
	}
}

int Server::getMaxFd() {
	
	int max = -1;

	std::vector<int>::iterator it = monitored_fds.begin();

	while (it != monitored_fds.end()){
		if (*it > max) max = *it;
		it++;
	}
	return max;
}


void Server::acceptNewClient() {

	struct sockaddr_storage clientAddr;
	socklen_t 				size = sizeof(clientAddr);

	fd_set readfds;
	setFds(&readfds);

	select(getMaxFd() + 1, &readfds, NULL, NULL, NULL);

	int status;

	if ((status = FD_ISSET(sockfd, &readfds))){
		std::cout << "status: " << status << std::endl;
		std::cout << "Connection request from a new client" << std::endl;

		int new_fd = accept(sockfd, (struct sockaddr *)&clientAddr, &size);
		if (new_fd == -1) {
			std::cout << "NEW_FD: ERROR" << std::endl;
		} else {
			clients[new_fd] = Client::createClient(clientAddr, size);
			clients[new_fd]->setNickname("Anastacia");
			std::cout << "Testing creation of Client: " << clients[new_fd]->getNickname() << std::endl;
			std::cout << "NEW_FD: OK" << std::endl;

			monitored_fds.push_back(new_fd);
			std::cout << "new_fd added to monitored_fds" << std::endl;

			clients[new_fd]->setTextAddr(inet_ntoa(get_in_addr((struct sockaddr *)&clientAddr)));
			std::cout << "got connection from " << clients[new_fd]->getTextAddr() << std::endl;
		}

	} else {

		std::cout << "status: " << status << std::endl;

		int actual_fd = -1;
		int data;

		std::vector<int>::iterator it = monitored_fds.begin();

		while (it != monitored_fds.end()) {
			if (FD_ISSET(*it, &readfds)){
				actual_fd = *it;

				memset(message, 0, BUFFER_SIZE);

				std::cout << "waiting for data" << std::endl;

				if (read(actual_fd, message, BUFFER_SIZE) >= 0) {
					memcpy(&data, message, sizeof(int));
					std::cout << "message: " << message << std::endl;
				}
				if (data == 0) {
					std::cout << "sending back to client" << std::endl;
					memset(message, 0, BUFFER_SIZE);
					write(actual_fd, message, BUFFER_SIZE);
					close(actual_fd);
					monitored_fds.erase(it);
					continue;
				}
			}

			it++;
		}
	}
	
}


