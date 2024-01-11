#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

#include "ftIrc.hpp"

class Client {

	private:
		struct sockaddr_storage	addr;
		socklen_t				size;
		std::string				username;
		std::string				nickname;

	public:
		Client(struct sockaddr_storage addr, socklen_t size);
		~Client();
		Client(Client const & src);
		Client & operator=(Client const & rhs);
		std::string getUsername() const;
		std::string getNickname() const;
		void setUsername(std::string name);
		void setNickname(std::string name);
		sockaddr_storage getAddr() const;
		socklen_t getSize() const;
		static Client * createClient(struct sockaddr_storage addr, socklen_t size);


};



#endif