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
		eventsCount = 1;
	} else {
		exit(PORT_NOT_VALID);
	}
	this->password = new Password(password);

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
	delete password;

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
	memset(&events, 0, sizeof(events));

	struct	epoll_event event;

	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = sockfd;
	events[0].data.fd = sockfd;

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
				std::map<int, Client*>::iterator client_it = clients.find(client_fd);
				if (client_it != clients.end()) {
					Client client = *client_it->second;
					client.setStatus(true);
					
					// if (events[i].events == EPOLLIN || EPOLLOUT) {
					// }
						receiveMessage(client);
					if (!client.getStatus()) {
						close(client_fd);
						epoll_ctl(efd, EPOLL_CTL_DEL, client_fd, &event);
						eventsCount--;
						continue;
					}
				}
		
			}
		}
	}
}

void Server::receiveMessage(Client & client) {
	memset(recv_buffer, 0, BUFFER_SIZE);
	int bytes_recv = 0;
	bytes_recv = recv(client.getFd(), recv_buffer, BUFFER_SIZE, 0);
	if (bytes_recv == -1) {
		error("ERROR READING SOCKET", false);
	}
	if (bytes_recv == 0) {
		client.setStatus(false);
		std::cout << "connection lost with client " << client.getTextAddr() << std::endl;
	}
	message.append(recv_buffer);
	sleep(2); // time to wait for the complete registration message from HexChat
	
	parseMessage(client);

	message.erase();
}

void Server::parseMessage(Client & client) {
	// netcat sends messages terminating in "\n", not in "\r\n"
	size_t end = message.find("\n");
	size_t start = 0;
	while (end != EOS) {
		std::string msg = message.substr(start, end);

		if (msg.find("PASS") != EOS) {
			authenticate(client);
		} else if (msg.find("USER") != EOS) {
			setClientUser(client);
		} else if (msg.find("NICK") != EOS) {
			setClientNick(client);
		} else {
			// Sending messages to all clients connected to the server, only to test multiclients
			for (int i = 0; i < 200; i++) {
				if (events[i].data.fd != client.getFd()) {
					// using stringstream to convert size_t fds to string.
					std::stringstream ss;
					ss << "SOCKET: " << events[i].data.fd << " :" << msg << "\r\n";
					std::string m = ss.str();
					send(events[i].data.fd, m.c_str(), m.size(), 0);
				}
			}
		}
		start = end + 1;
		if (start != EOS) end = message.find("\n", start);
		else end = start;
	}
}

void Server::setClientUser(Client & client) {
	size_t pos = message.find("USER");
	size_t start, end;
	if (pos != EOS) {
		start = pos + 5;
		end = message.find(" ", start);
		client.setUsername(message.substr(start, end - start));
	}
}

void Server::setClientNick(Client & client) {
	size_t pos = message.find("NICK");
	size_t start, end;
	if (pos != EOS) {
		start = pos + 5;
		end = message.find("\n", start);
		client.setNickname(message.substr(start, end - start));
		client.registration(true);
		sendRPL(client);
	}
}

void Server::authenticate(Client & client) {
	
	size_t pos = message.find("PASS");
		if(pos != EOS) {
			size_t end = message.find("\n", pos);
			if (message.at(end - 1) == '\r') end = end - 1;
			size_t start = pos + 5;
			std::string pass = message.substr(start, end - start); // we need to eliminate the \n on the end of the message
			client.setAuthentication(password->validate(pass));
			if (!client.isAuthenticated()) sendWarning("Wrong password!\r\n", client);
			else {
				setClientUser(client);
				setClientNick(client);
			}
		}
		else {
			sendWarning("Client authentication needed!\r\n", client); // we need to check the correct way to warn it!
		}
}

void Server::sendWarning(std::string msg, Client & client) {
	send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}


void Server::acceptNewClient() {

	
	struct sockaddr_storage clientAddr;
	socklen_t 				size = sizeof(clientAddr);

	std::cout << std::endl;
	std::cout << "Connection request from a new client" << std::endl;

	int new_fd = accept(sockfd, (struct sockaddr *)&clientAddr, &size);
	error("CREATING CLIENT FD", new_fd != -1);
	
	if (new_fd != -1) {
		clients[new_fd] = Client::createClient(clientAddr, size, new_fd);

		fcntl(new_fd, F_SETFL, O_NONBLOCK);
		struct	epoll_event event;
		event.events = EPOLLIN;
		// this way it only accept one client each time
		// event.events = EPOLLIN | EPOLLOUT;
		event.data.fd = new_fd;

		error("ADDING CLIENT TO POLL", epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, &event) != -1);
		events[eventsCount] = event;
		eventsCount++;

		std::cout << "new_fd added to monitored_fds" << std::endl;
		clients[new_fd]->setTextAddr(inet_ntoa(get_in_addr((struct sockaddr *)&clientAddr)));
		std::cout << "Got connection from " << clients[new_fd]->getTextAddr() << " on " << new_fd << std::endl;
	
	}	
}

in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}


// to improve!

void Server::sendRPL(Client & client) {

	std::string welcome = RPL_WELCOME(client.getNickname());
	send(client.getFd(), welcome.c_str(), welcome.size(), MSG_NOSIGNAL);

	std::string yourHost = RPL_YOURHOST(client.getNickname());
	send(client.getFd(), yourHost.c_str(), yourHost.size(), MSG_NOSIGNAL);

	std::string created = RPL_CREATED(client.getNickname());
	send(client.getFd(), created.c_str(), created.size(), MSG_NOSIGNAL);

	// // include list of user modes and channels
	std::string myInfo = RPL_MYINFO(client.getNickname());
	send(client.getFd(), myInfo.c_str(), myInfo.size(), MSG_NOSIGNAL);




}


