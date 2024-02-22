#include "../includes/Channel.hpp"

/* ------------------------------ Constructers ------------------------------ */

Channel::Channel()	{
	_name = "";
	_password = "";

	_mode._invite_only = false;
	_mode._key = false;
	_mode._protected_topic = false;
	_mode._user_limit = false;
}

Channel::Channel(std::string name)	{
	_name = name;
	_password = "";

	_mode._invite_only = false;
	_mode._key = false;
	_mode._protected_topic = false;
	_mode._user_limit = false;
}

Channel::Channel(std::string name, std::string password)	{
	_name = name;
	_password = password;

	_mode._invite_only = false;
	_mode._key = false;
	_mode._protected_topic = false;
	_mode._user_limit = false;
}

Channel::~Channel()	{
}

Channel::Channel(Client const & src)	{
	*this = src;
}

Channel & Channel::operator=(Channel const & rhs) {

	_name = rhs.getName();
	_password = rhs.getPassword();
	_mode = rhs.getMode();
	_clients = rhs.getClients();

	return *this;

}

/* --------------------------------- Getters -------------------------------- */

std::string Channel::getName() const	{
	return _name;
}

std::string Channel::getPassword() const	{
	return _password;
}

t_mode_channel Channel::getMode() const	{
	return _mode;
}

std::vector<Client> Channel::getClients() const	{
	return _clients;
}

std::vector<Client> Channel::getOperators() const	{
	return _clients;
}

long unsigned int	Channel::getUserLimit() const	{
	return _user_limit;
}

/* --------------------------------- setters -------------------------------- */

void	Channel::setName(std::string name){
	_name = name;
}

void	Channel::setPassword(std::string password){
	_password = password;
}

/* --------------------------------- Methods -------------------------------- */

void	Channel::addClient(Client & client)	{
	_clients.push_back(client);
}

void	Channel::removeClient(Client & client) {
	std::vector<Client>::iterator	pos = findClient(client);

	if (pos != _clients.end())
		_clients.erase(findClient(client));
	// todo: error msg
	// else
}

std::vector<Client>::iterator	Channel::findClient(Client const & client)	{
	for (unsigned long i = 0; i < _clients.size(); i++)	{
		if (client.getFd() == _clients.at(i).getFd()) {
			return (_clients.begin() + i);
		}
	}
	return (_clients.end());
}

void	Channel::addMode(char mode, std::string parameters) {

	switch (mode)	{

		case 'i':
			_mode._invite_only = true;
			return ;
		case 'k':
			_mode._key = true;
			_password = parameters;
			return ;
		case 't':
			_mode._protected_topic = true;
			return ;
		case 'n':
			_mode._user_limit = true;
			_user_limit = atoi(parameters.c_str());
			return ;
		case 'o':

			int c = -1;
			for (unsigned long i = 0; i < _clients.size(); i++) {

				if (_clients.at(i).getNickname() == parameters) {
					c = i;
				}
					
			}
			if (c != -1)	{
				_operators.push_back(_clients.at(c));
			}	else	{
				//!ERROR
			}

			return ;

	}
	//!error msg, mode not found

}

void	Channel::removeMode(char mode, std::string parameters) {

	switch (mode)	{

		case 'i':
			_mode._invite_only = false;
			return ;
		case 'k':
			_mode._key = false;
			return ;
		case 't':
			_mode._protected_topic = false;
			return ;
		case 'n':
			_mode._user_limit = false;
			return ;
		case 'o':

			int c = -1;
			for (unsigned long i = 0; i < _clients.size(); i++) {

				if (_operators.at(i).getNickname() == parameters) {
					c = i;
				}
					
			}
			if (c != -1)	{
				_operators.erase(_operators.begin() + c);
			}	else	{
				//!ERROR
			}

			return ;

	}
	//!error msg, mode not found

}