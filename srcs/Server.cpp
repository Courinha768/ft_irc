#include "../includes/Server.hpp"

Server::Server(std::string port, std::string password) {
	if (isPortValid(port)) {
		this->port = port;
	} else {
		exit(1);
	}
	this->password = new Password(password);
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
	delete password;
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

	std::cout << "pass: " << password->getPassword() << std::endl;
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

	setupPoll();

}

in_addr Server::get_in_addr(struct sockaddr *sa){

	return (((struct sockaddr_in*)sa)->sin_addr);

}

void Server::setupPoll() {

	efd = epoll_create1(0);
	if (efd == -1) {
		std::cout << "error creating epoll()" << std::endl;
	}

	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = sockfd;

	if (epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
		std::cout << "error adding server socket to epoll instance" << std::endl;
	}

	while (1) {
		int numEvents = epoll_wait(efd, events, 200, -1);

		if (numEvents == -1) {
			std::cout << "failed waiting for events" << std::endl;
			break;
		}

		for (int i = 0; i < numEvents; ++i) {
			if (events[i].data.fd == sockfd) {

				// new client
				acceptNewClient();

			} else { // client already registered
				bool connectionUp = true;
				int client_fd = events[i].data.fd;
				std::map<int, Client*>::iterator client = clients.find(client_fd);

				if (client->second->isAuthenticated()) {
					if (client != clients.end()) {
						client->second->handleCommunication(client_fd, &connectionUp);
						//just to test using of send()
						send(client_fd, "Received!!\n", 11, MSG_NOSIGNAL);
					}
					if (!connectionUp) {
						close(client_fd);
						epoll_ctl(efd, EPOLL_CTL_DEL, client_fd, &event);
						continue;
					}
				} else {
					//password->validate()
				}

			}
		}
	}
}



void Server::acceptNewClient() {

	
	struct sockaddr_storage clientAddr;
	socklen_t 				size = sizeof(clientAddr);

	std::cout << "Connection request from a new client" << std::endl;

	int new_fd = accept(sockfd, (struct sockaddr *)&clientAddr, &size);
	if (new_fd == -1) {
		std::cout << "NEW_FD: ERROR" << std::endl;
	} else {
		clients[new_fd] = Client::createClient(clientAddr, size);
		clients[new_fd]->setNickname("Anastacia");
		std::cout << "Testing creation of Client: " << clients[new_fd]->getNickname() << std::endl;
		std::cout << "NEW_FD: OK" << std::endl;

		event.events = EPOLLIN;
		event.data.fd = new_fd;
		if (epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, &event) == -1) {
			std::cout << "error adding new client" << std::endl;
		} 
		std::cout << "new_fd added to monitored_fds" << std::endl;
		clients[new_fd]->setAuthentication(true);
		clients[new_fd]->setTextAddr(inet_ntoa(get_in_addr((struct sockaddr *)&clientAddr)));
		std::cout << "got connection from " << clients[new_fd]->getTextAddr() << std::endl;
	}
	
	
}



