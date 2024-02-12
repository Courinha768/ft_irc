#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ftIrc.hpp"

typedef struct s_mode	{
	bool	_ban;
	bool	_exeption;
	bool	_client_limit;
	bool	_invite_only;
	bool	_invite_exeption;
	bool	_key;
	bool	_moderated;
	bool	_secret;
	bool	_protected;
	bool	_no_external_messages;
} t_mode;

class	Channel	{

	private:
		std::string			_name;
		std::string 		_password;
		t_mode				_mode;
		std::vector<Client>	_clients;

	public:

		Channel();
		Channel(std::string name);
		Channel(std::string name, std::string password);
		// Channel(std::string name, std::string password, std::string modes);
		~Channel();
		Channel(Client const & src);

		Channel & operator=(Channel const & rhs);

		std::string			getName() const;
		std::string			getPassword() const;
		t_mode				getMode() const;
		std::vector<Client>	getClients() const;

		void	setName(std::string name);
		void	setPassword(std::string password);

		void				addClient(Client & client);
		void				removeClient(Client & client);

		std::vector<Client>::iterator	findClient(Client const & client);

};



#endif