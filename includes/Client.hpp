#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

#include "ftIrc.hpp"

class Client {

	private:
		struct sockaddr_storage	addr;
		socklen_t				size;
		std::string				text_addr;
		std::string				username;
		std::string				nickname;
		char					message[BUFFER_SIZE];

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
		std::string getTextAddr() const;
		void setTextAddr(std::string addr);
		static Client * createClient(struct sockaddr_storage addr, socklen_t size);

		void handleCommunication(int fd, bool * connectionUp);


};



#endif