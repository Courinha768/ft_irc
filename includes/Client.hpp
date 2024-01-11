#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

#include "ftIrc.hpp"

class Client {

	private:
		struct sockaddr_storage	addr;
		socklen_t				size;

	public:
		Client(struct sockaddr_storage addr, socklen_t size);
		~Client();
		Client(Client const & src);
		Client & operator=(Client const & rhs);
		sockaddr_storage getAddr() const;
		socklen_t getSize() const;
		void setSize(socklen_t size);
		static Client * createClient(struct sockaddr_storage addr, socklen_t size);


};



#endif