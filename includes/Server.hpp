#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include "ftIrc.hpp"
# include "Client.hpp"
# include "Password.hpp"

class Client;
class Password;

class Server {

	private:

		std::string		port;
		Password		*password;
		struct addrinfo	serv;
		struct addrinfo	*servinfo;
		int				status;
		int				sockfd;

		std::map<int, Client *>	clients;
		char			recv_buffer[BUFFER_SIZE];
		std::string		message;
		// struct	epoll_event event;
		// std::vector<epoll_event> events;
		struct	epoll_event events[200];
		int		efd;

	public:

		Server(std::string port, std::string password);
		Server(Server const & src);
		~Server();
		
		Server & operator=(Server const & rhs);
		
		void setup();
		in_addr get_in_addr(struct sockaddr *sa);
		void acceptNewClient();
		bool authentication(std::string pass);
		void setupPoll();
		void receiveMessage(int fd, Client & client);

};


#endif