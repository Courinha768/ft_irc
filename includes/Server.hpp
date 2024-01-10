#ifndef _SERVER_HPP_
# define _SERVER_HPP_

#include "ftIrc.hpp"

class Server {

	private:
		std::string		port;
		std::string		password;
		struct addrinfo	serv;
		struct addrinfo	*servinfo;
		int				status;
		int				sockfd;

	public:
		Server(std::string port, std::string password);
		~Server();
		Server(Server const & src);
		Server & operator=(Server const & rhs);
		static bool isPortValid(std::string port);
		void setup();
		void initialize_server();

};


#endif