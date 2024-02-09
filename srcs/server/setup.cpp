#include "../../includes/Server.hpp"

void Server::setup() {

	//todo: take care of this errors
	memset(&serv, 0, sizeof(serv));

	serv.ai_family		= AF_INET;
	serv.ai_socktype	= SOCK_STREAM;
	serv.ai_protocol	= getprotobyname("TCP")->p_proto;

	int option_value = 1;

	status = getaddrinfo("0.0.0.0", port.c_str(), &serv, &servinfo);
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
	bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	listen(sockfd, 5);

	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = sockfd;
	events[0].data.fd = sockfd;

	efd = epoll_create1(0);
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

				if (client_it != clients.end()) {
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

void Server::acceptNewClient() {

	struct sockaddr_storage clientAddr;
	socklen_t 				size = sizeof(clientAddr);

	Server::cout() << NL << "Connection request from a new client" << NL;

	int new_fd = accept(sockfd, (struct sockaddr *)&clientAddr, &size);
	error("CREATING CLIENT FD", new_fd != -1);
	
	if (new_fd != -1) {

		clients[new_fd] = Client::createClient(clientAddr, size, new_fd);

		fcntl(new_fd, F_SETFL, O_NONBLOCK);
		struct	epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = new_fd;

		error("ADDING CLIENT TO POLL", epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, &event) != -1);
		events[eventsCount] = event;
		eventsCount++;

		Server::cout() << "new_fd added to monitored_fds" << NL;
		clients[new_fd]->setTextAddr(inet_ntoa(get_in_addr((struct sockaddr *)&clientAddr)));
		Server::cout() << "Got connection from " << clients[new_fd]->getTextAddr() << " on " << new_fd << NL << NL;

	}
}

in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}


