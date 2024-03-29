#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

#include "ftIrc.hpp"

typedef struct s_mode_client	{
	bool	_registered;
} t_mode_client;
class Client {

	private:
		struct sockaddr_storage	addr;
		socklen_t				size;
		int						fd;
		std::string				text_addr;
		std::string				username;
		std::string				realname;
		std::string				nickname;
		bool					authenticated;
		bool					status;
		t_mode_client			modes;


	public:

		Client(struct sockaddr_storage addr, socklen_t size, int fd);
		~Client();
		Client(Client const & src);

		Client & operator=(Client const & rhs);
		bool	operator==(Client const & rhs);

		std::string			getUsername() const;
		std::string			getRealname() const;
		std::string			getNickname() const;
		bool 				hasNick() const;
		bool				hasUser() const;
		int					getFd() const;
		socklen_t			getSize() const;
		bool				getStatus() const;
		sockaddr_storage	getAddr() const;
		std::string			getTextAddr() const;
		bool				isAuthenticated() const;
		bool				isRegistered() const;
		bool				isOperator(Channel channel) const;

		void	setUsername(std::string name);
		void	setRealname(std::string name);
		void	setNickname(std::string name);
		void	setTextAddr(std::string addr);
		void	setAuthentication(bool status);
		void	setStatus(bool status);
		void	setisRegistered(bool status);

		static Client * createClient(struct sockaddr_storage addr, socklen_t size, int fd);



};



#endif