#include "../includes/Channel.hpp"

/* ------------------------------ Constructers ------------------------------ */

Channel::Channel()	{
	_name = "";
	_password = "";

	_mode._ban = false;
	_mode._exeption = false;
	_mode._client_limit = false;
	_mode._invite_only = false;
	_mode._invite_exeption = false;
	_mode._key = false;
	_mode._moderated = false;
	_mode._secret = false;
	_mode._protected = false;
	_mode._no_external_messages = false;
}

Channel::Channel(std::string name)	{
	_name = name;
	_password = "";

	_mode._ban = false;
	_mode._exeption = false;
	_mode._client_limit = false;
	_mode._invite_only = false;
	_mode._invite_exeption = false;
	_mode._key = false;
	_mode._moderated = false;
	_mode._secret = false;
	_mode._protected = false;
	_mode._no_external_messages = false;
}

Channel::Channel(std::string name, std::string password)	{
	_name = name;
	_password = password;

	_mode._ban = false;
	_mode._exeption = false;
	_mode._client_limit = false;
	_mode._invite_only = false;
	_mode._invite_exeption = false;
	_mode._key = false;
	_mode._moderated = false;
	_mode._secret = false;
	_mode._protected = false;
	_mode._no_external_messages = false;
}

// Channel::Channel(std::string name, std::string password, std::string modes)	{
// 	_name = name;
// 	_password = password;

// 	if (modes.find("+b") != EOS)
// 		_mode._ban = true;
// 	if (modes.find("+e") != EOS)
// 		_mode._exeption = true;
// 	if (modes.find("+l") != EOS)
// 		_mode._client_limit = true;
// 	if (modes.find("+i") != EOS)
// 		_mode._invite_only = true;
// 	if (modes.find("+I") != EOS)
// 		_mode._invite_exeption = true;
// 	if (modes.find("+k") != EOS)
// 		_mode._key = true;
// 	if (modes.find("+m") != EOS)
// 		_mode._moderated = true;
// 	if (modes.find("+s") != EOS)
// 		_mode._secret = true;
// 	if (modes.find("+t") != EOS)
// 		_mode._protected = true;
// 	if (modes.find("+n") != EOS)
// 		_mode._no_external_messages = true;
// }

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

t_mode Channel::getMode() const	{
	return _mode;
}

std::vector<Client> Channel::getClients() const	{
	return _clients;
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
		if (client.getFd() == _clients.at(i).getFd())
			return (_clients.begin() + i);
	}
	return (_clients.end());
}