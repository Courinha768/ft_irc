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
		struct	epoll_event events[200];
		int					eventsCount;
		int		efd;

	public:

		Server(std::string port, std::string password);
		~Server();
		
		void setup();
		in_addr get_in_addr(struct sockaddr *sa);
		void acceptNewClient();
		bool authentication(std::string pass);
		void setupPoll();
		void receiveMessage(Client & client);
		void parseMessage(Client & client);
		void authenticate(Client & client);
		void sendWarning(std::string msg, Client & client);
		void setClientUser(Client & client);
		void setClientNick(Client & client);
		void sendRPL(Client & client);

		//! maybe change this name
		typedef typename std::map<int, Client*>::iterator client_iterator;

};


#endif