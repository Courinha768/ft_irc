#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

#include "ftIrc.hpp"

class Client {

	private:
		struct sockaddr_storage	addr;
		socklen_t				size;
		int						fd;
		std::string				text_addr;
		std::string				username;
		std::string				nickname;
		char					message[BUFFER_SIZE];
		bool					authenticated;
		bool					registered;
		bool					status;

	public:
		Client(struct sockaddr_storage addr, socklen_t size, int fd);
		~Client();
		Client(Client const & src);
		Client & operator=(Client const & rhs);
		std::string getUsername() const;
		std::string getNickname() const;
		void setUsername(std::string name);
		void setNickname(std::string name);
		sockaddr_storage getAddr() const;
		socklen_t getSize() const;
		int getFd() const;
		std::string getTextAddr() const;
		void setTextAddr(std::string addr);
		bool isAuthenticated() const;
		void setAuthentication(bool status);
		bool isRegistered() const;
		void registration(bool status);
		void setStatus(bool status);
		bool getStatus() const;
		static Client * createClient(struct sockaddr_storage addr, socklen_t size, int fd);



};



#endif