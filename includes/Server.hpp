#ifndef _SERVER_HPP_
# define _SERVER_HPP_

#include "ftIrc.hpp"

struct s_socket
{
	int		fd;
	struct	sockaddr_in info;
};

class Server {

	private:
		int			port;
		s_socket	servinfo;
		std::string	password;

	public:
		Server(int port, std::string password);
		~Server();
		Server(Server const & src);
		Server & operator=(Server const & rhs);

		void initialize_server();

};


#endif