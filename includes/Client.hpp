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
		bool					_hasUser;
		bool					_isRegistered;
		bool					authenticated;
		bool					status;

	public:

		Client(struct sockaddr_storage addr, socklen_t size, int fd);
		~Client();
		Client(Client const & src);

		Client & operator=(Client const & rhs);

		std::string			getUsername() const;
		std::string			getNickname() const;
		int					getFd() const;
		socklen_t			getSize() const;
		bool				getStatus() const;
		sockaddr_storage	getAddr() const;
		std::string			getTextAddr() const;
		bool				isAuthenticated() const;
		bool				hasUser() const;
		bool				isRegistered() const;

		void	setUsername(std::string name);
		void	setNickname(std::string name);
		void	setTextAddr(std::string addr);
		void	setAuthentication(bool status);
		void	setStatus(bool status);
		void	setHasUser(bool status);
		void	setisRegistered(bool status);

		static Client * createClient(struct sockaddr_storage addr, socklen_t size, int fd);



};



#endif