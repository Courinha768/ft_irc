#include "../../includes/ftIrc.hpp"

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