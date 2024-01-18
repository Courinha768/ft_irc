#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include "ftIrc.hpp"
# include "Client.hpp"

class Client;

class Server {

	private:

		std::string		port;
		std::string		password;
		struct addrinfo	serv;
		struct addrinfo	*servinfo;
		int				status;
		int				sockfd;

		std::map<int, Client *>	clients;

		char					message[BUFFER_SIZE];
		struct	epoll_event 	event;
		struct	epoll_event 	events[200];
		int						efd;

	public:

		Server(std::string port, std::string password);
		Server(Server const & src);
		~Server();
		
		Server & operator=(Server const & rhs);
		
		void setup();
		in_addr get_in_addr(struct sockaddr *sa);
		void acceptNewClient();
		void setupPoll();

};


#endif