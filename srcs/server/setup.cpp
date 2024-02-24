#include "../../includes/ftIrc.hpp"

void Server::setup() {
	
	memset(&serv, 0, sizeof(serv));

	serv.ai_family		= AF_INET;
	serv.ai_socktype	= SOCK_STREAM;
	serv.ai_protocol	= getprotobyname("TCP")->p_proto;

	int option_value = 1;

	status = getaddrinfo("0.0.0.0", port.c_str(), &serv, &servinfo);
	if (status != 0) {
		throw std::runtime_error("Error initializing addrinfo structures.");
	}
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd == -1) {
		throw std::runtime_error("Error creating endpoint for communication.");
	}
	status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
	if (status == -1) {
		throw std::runtime_error("Error setting socket options.");
	}
	status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if (status == -1) {
		throw std::runtime_error("Error binding socket.");
	}
	status = listen(sockfd, 5);
	if (status == -1) {
		throw std::runtime_error("Error listening on socket.");
	}

	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = sockfd;
	events[0].data.fd = sockfd;

	efd = epoll_create1(0);
	if (efd == -1) {
		throw std::runtime_error("Error opening epoll file descriptor.");
	}
	epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event);

	memset(&events, 0, sizeof(events));

	Server::cout() << BCYN << "Server:" << CRESET;
	std::cout << std::endl;
}

void Server::setupPoll() {

	while (true) {

		int numEvents = epoll_wait(efd, events, 200, -1);
		if (numEvents == EPPOL_WAIT_ERROR) {
			Server::cout() << RED << "ERROR: " << CRESET << "failed waiting for events" << NL;
			break;
		}

		
		for (int i = 0; i < numEvents; ++i) {

			if (events[i].data.fd == sockfd) {
				
				acceptNewClient();

			} else {

				int									client_fd = events[i].data.fd;
				std::map<int, Client*>::iterator	client_it = clients.find(client_fd);

				if (client_it->second && client_it != clients.end()) {
					(*client_it->second).setStatus(true);
					
					receiveMessage((*client_it->second));
					

					if (!(*client_it->second).getStatus()) {
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
