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
		std::vector<int> monitored_fds;

	public:
		Server(std::string port, std::string password);
		~Server();
		Server(Server const & src);
		Server & operator=(Server const & rhs);
		static bool isPortValid(std::string port);
		void setup();
		void initialize_server();
		in_addr get_in_addr(struct sockaddr *sa);
		void acceptNewClient();
		void setFds(fd_set *ptr);
		int getMaxFd();

};


#endif