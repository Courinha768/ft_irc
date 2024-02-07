#include "../../includes/Server.hpp"

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

	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = sockfd;
	events[0].data.fd = sockfd;

	error("EPOLL"			, (efd = epoll_create1(0)) != -1);
	error("ADDING TO EPPOL"	, (epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event) != -1));

	memset(&events, 0, sizeof(events));
}

void Server::setupPoll() {

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
				std::map<int, Client*>::iterator	client_it = clients.find(client_fd);

				if (client_it != clients.end()) {
					Client client = *client_it->second;
					client.setStatus(true);
					
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
		event.data.fd = new_fd;

		error("ADDING CLIENT TO POLL", epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, &event) != -1);
		events[eventsCount] = event;
		eventsCount++;

		std::cout << "new_fd added to monitored_fds" << std::endl;
		clients[new_fd]->setTextAddr(inet_ntoa(get_in_addr((struct sockaddr *)&clientAddr)));
		std::cout << "Got connection from " << clients[new_fd]->getTextAddr() << " on " << new_fd << std::endl << std::endl;

	}	
}

in_addr Server::get_in_addr(struct sockaddr *sa){
	return (((struct sockaddr_in*)sa)->sin_addr);
}


