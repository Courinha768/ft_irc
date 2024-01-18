#include "../includes/Server.hpp"

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
	} else {
		exit(PORT_NOT_VALID);
	}
	this->password = password;

}

//todo: Finnish this
Server::Server(Server const & src) {
	(void)src;
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

//todo: Finnish this
Server & Server::operator=(Server const & rhs) {
	(void)rhs;
	return *this;
}

void Server::setup() {

	memset(&serv, 0, sizeof(serv));

	serv.ai_family = AF_INET;
	serv.ai_socktype = SOCK_STREAM;
	serv.ai_protocol = getprotobyname("TCP")->p_proto;

	int option_value = 1;

	error("GETADDRINFO", (status = getaddrinfo("0.0.0.0", port.c_str(), &serv, &servinfo)) == 0);
	error("SOCKET", (sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) != -1);
	error("SET SOCKET", setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) != -1);
	error("BIND", (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) == 0);
	error("SOCKET", (listen(sockfd, 5)) != -1);

}

void Server::setupPoll() {

	event.events = EPOLLIN;
	event.data.fd = sockfd;

	error("EPOLL", (efd = epoll_create1(0)) != -1);
	error("ADDING TO EPPOL", (epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event) != -1));

	while (true) {

		int numEvents = epoll_wait(efd, events, 200, -1);
		if (numEvents == -1) {
			std::cout << RED << "ERROR: " << CRESET << "failed waiting for events" << std::endl;
			break;
		}

		for (int i = 0; i < numEvents; ++i) {

			if (events[i].data.fd == sockfd) { // new client

				acceptNewClient();

			} else { // client already registered

				int client_fd = events[i].data.fd;
				int data;

				//? What is this doing
				memset(message, 0, BUFFER_SIZE);

				if (read(client_fd, message, BUFFER_SIZE) >= 0) {
					//? Why are we cpy the memory to data
					memcpy(&data, message, sizeof(int));
					//todo: find the name of the client sending the message
					std::cout << "message: " << message << std::endl;
				}
				if (data == 0) {
					std::cout << "Client leaving" << std::endl;
					//? What is this doing (same as before?)
					memset(message, 0, BUFFER_SIZE);
					//? Why are we writing BUFFER_SIZE and not the size of the message
					//? Why does it print 2 \n's
					write(client_fd, message, BUFFER_SIZE);
					close(client_fd);

					epoll_ctl(efd, EPOLL_CTL_DEL, client_fd, &event);
				}

			}
		}
	}
}



void Server::acceptNewClient() {

	
	struct sockaddr_storage clientAddr;
	socklen_t 				size = sizeof(clientAddr);

	std::cout << std::endl;
	std::cout << "Connection request from a new client" << std::endl;

	int new_fd = accept(sockfd, (struct sockaddr *)&clientAddr, &size);
	if (new_fd == -1) {
		std::cout << "NEW_FD: ERROR" << std::endl;
	} else {
		//? Why are we creating a function that is only used here and can be replaced by "new Client(clientAddr, size)"
		clients[new_fd] = Client::createClient(clientAddr, size);
		//todo: find how to get nickname from netcat
		clients[new_fd]->setNickname("Anastacia");
		std::cout << "Creation of Client: " << clients[new_fd]->getNickname() << std::endl;

		event.events = EPOLLIN;
		event.data.fd = new_fd;

		if (epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, &event) == -1) {
			std::cout << "error adding new client" << std::endl;
		}
		std::cout << clients[new_fd]->getNickname() << "'fd added to monitored_fds" << std::endl;

		clients[new_fd]->setTextAddr(inet_ntoa(get_in_addr((struct sockaddr *)&clientAddr)));
		std::cout << "Got connection from " << clients[new_fd]->getTextAddr() << std::endl;
		std::cout << std::endl;
	}	
}

in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}
