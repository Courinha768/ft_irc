#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ftIrc.hpp"

typedef struct s_mode_channel	{
	bool	_invite_only;
	bool	_key;
	bool	_protected_topic;
	bool	_user_limit;
} t_mode_channel;

class	Channel	{

	private:
		std::string			_name;
		std::string 		_password;
		std::string			_topic;
		t_mode_channel		_mode;
		std::vector<Client>	_clients;
		std::vector<Client>	_invited;
		std::vector<Client>	_operators;
		long unsigned int	_user_limit;

	public:

		Channel();
		Channel(std::string name);
		Channel(std::string name, std::string password);
		~Channel();
		Channel(Client const & src);

		Channel & operator=(Channel const & rhs);

		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getPassword() const;
		t_mode_channel		getMode() const;
		std::vector<Client>	getClients() const;
		std::vector<Client>	getInviteds() const;
		std::vector<Client>	getOperators() const;
		long unsigned int	getUserLimit() const;


		void				setName(std::string name);
		void				setPassword(std::string password);
		void				setTopic(std::string topic);

		void				addClient(Client & client);
		void				addInvited(Client & client);
		void				addOperator(Client & client);
		void				removeClient(Client & client);
		void				removeInvited(Client & client);

		void				addMode(char mode, std::string parameters);
		void				removeMode(char mode, std::string parameters);

		std::vector<Client>::iterator	findClient(Client const & client);
		std::vector<Client>::iterator	findOperators(Client const & client);
		std::vector<Client>::iterator	findInvited(Client const & client);

};



#endif