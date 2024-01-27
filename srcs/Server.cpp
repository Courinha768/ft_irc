#include "../includes/Server.hpp"

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

/**
 * This method configures the Server by performing the following steps:
 * - Initializes the 'serv' structure and sets relevant parameters.
 * - Retrieves the TCP protocol and sets it for the 'serv' structure.
 * - Initializes option_value for socket options.
 * - Retrieves address information for the server using getaddrinfo.
 * - Creates a socket using the obtained address information.
 * - Sets the socket option SO_REUSEADDR.
 * - Binds the socket to the specified address and port.
 * - Listens on the socket for incoming connections with a maximum backlog of 5.
 * 
 * This method is responsible for setting up the server to start accepting client connections.
 */
void Server::setup() {

	memset(&serv, 0, sizeof(serv));

	serv.ai_family		= AF_INET;
	serv.ai_socktype	= SOCK_STREAM;
	serv.ai_protocol	= getprotobyname("TCP")->p_proto;

	int option_value = 1;

	error("GETADDRINFO"	, (status = getaddrinfo("0.0.0.0", port.c_str(), &serv, &servinfo)) == 0);
	error("SOCKET"		, (sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) != -1);
	error("SET SOCKET"	, setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) != -1);
	error("BIND"		, (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) == 0);
	error("SOCKET"		, (listen(sockfd, 5)) != -1);

}

/**
 * This method continuously waits for events and handles them accordingly:
 * - If a new client connection event is detected, it calls the acceptNewClient method.
 * - If the event is associated with an already registered client, it updates the client's
 *   status and processes incoming data if available.
 * - If a client's status indicates disconnection, it closes the client's file descriptor,
 *   removes the client from the epoll set, and continues to the next event.
 * 
 * This method ensures that the Server can efficiently manage and respond to various events,
 * such as new client connections and data reception from existing clients.
 */
void Server::setupPoll() {

	//! tried moving all this part to the setup function but couldn't
	struct	epoll_event event;

	event.events	= EPOLLIN | EPOLLOUT;
	event.data.fd	= sockfd;

	error("EPOLL"			, (efd = epoll_create1(0)) != -1);
	error("ADDING TO EPPOL"	, (epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event) != -1));
	//!end of "this part"

	while (true) {

		int numEvents = epoll_wait(efd, events, 200, -1);
		if (numEvents == EPPOL_WAIT_ERROR) {
			std::cout << RED << "ERROR: " << CRESET << "failed waiting for events" << std::endl;
			break;
		}

		for (int i = 0; i < numEvents; ++i) {

			if (events[i].data.fd == sockfd) { // new client

				acceptNewClient();

			} else { // client already registered

				int				client_fd = events[i].data.fd;
				client_iterator	client_it = clients.find(client_fd);

				if (client_it != clients.end()) {
					Client client = *client_it->second;
					client.setStatus(true);
					
					if (events[i].events == EPOLLIN) {
						receiveMessage(client);
					}

					if (!client.getStatus()) {
						close(client_fd);
						epoll_ctl(efd, EPOLL_CTL_DEL, client_fd, &event);
						continue;
					}
				}
		
			}
		}
	}
}

/**
 * Receives and processes messages from a connected client.
 *
 * @param client Reference to the connected client.
 */
void Server::receiveMessage(Client & client) {

	memset(recv_buffer, 0, BUFFER_SIZE);

	int bytes_recv = recv(client.getFd(), recv_buffer, BUFFER_SIZE, 0);
	if (bytes_recv == -1) {
		std::cout << BRED << "ERROR READING SOCKET" << CRESET << std::endl;
	}
	
	if (bytes_recv == 0) {
		client.setStatus(false);
		std::cout << "connection lost with client " << client.getTextAddr() << std::endl;
	}
	message.append(recv_buffer);
	
	if (!client.isAuthenticated()) {
		authenticate(client);
	} else {
		std::cout << message << std::endl;
	}
	
}

/**
 * Processes authentication messages from the client:
 * - Searches for the "PASS" keyword in the received message. If found, extracts the password from the message and validates it. Sets client authentication status based on the validation result.
 * - Sends a warning to the client for incorrect passwords.
 * - If "PASS" is not found, sends a warning about the need for client authentication.
 *
 * @param client Reference to the connected client.
 */
void Server::authenticate(Client & client) {

	size_t pos = message.find("PASS");

	if(pos != std::string::npos) {

		size_t end = message.find("\n", pos);
		size_t start = pos + 5;
		std::string pass = message.substr(start, end - start); // we need to eliminate the \n on the end of the message
		client.setAuthentication(password->validate(pass));
		if (!client.isAuthenticated())
			sendWarning("Wrong password!\n", client);
	}
	else {
		sendWarning("Client authentication needed! (write \"PASS <password>\")\n", client);
	}
	message.erase();
}


/**
 * Sends a warning message to the specified client's socket.
 *
 * @param msg The warning message to be sent.
 * @param client Reference to the target client.
 */
void Server::sendWarning(std::string msg, Client & client) {
	send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

/**
 * Accepts a connection request from a new client:
 * - Creates a new file descriptor (new_fd) for the client.
 * - Adds the new client to the server's clients map.
 * - Creates an epoll event for the new client and adds it to the epoll set.
 */
void Server::acceptNewClient() {

	struct sockaddr_storage clientAddr;
	socklen_t 				size = sizeof(clientAddr);

	std::cout << std::endl;
	std::cout << "Connection request from a new client" << std::endl;

	int new_fd = accept(sockfd, (struct sockaddr *)&clientAddr, &size);
	error("CREATING CLIENT FD", new_fd != -1);
	
	if (new_fd != -1) {
		clients[new_fd] = Client::createClient(clientAddr, size, new_fd);

		struct	epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = new_fd;

		error("ADDING CLIENT TO POLL", epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, &event) != -1);

		std::cout << "new_fd added to monitored_fds" << std::endl;
		clients[new_fd]->setTextAddr(inet_ntoa(get_in_addr((struct sockaddr *)&clientAddr)));
		std::cout << "Got connection from " << clients[new_fd]->getTextAddr() << std::endl;
	}	
}

/**
 * Retrieves the IPv4 address from a sockaddr structure.
 *
 * @param sa Pointer to the sockaddr structure.
 * @returns IPv4 address.
 */
in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}



