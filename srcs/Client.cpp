#include "../includes/Client.hpp"

Client::Client(struct sockaddr_storage addr, socklen_t size, int fd) : addr(addr), size(size), fd(fd) {
	authenticated = false;
	_hasUser = false;
	_hasNick = false;
}

Client::~Client() {}

Client::Client(Client const & src) {
	*this = src;
}

Client & Client::operator=(Client const & rhs) {
	this->addr = rhs.getAddr();
	this->size = rhs.getSize();
	this->fd = rhs.getFd();
	this->username = rhs.getUsername();
	this->nickname = rhs.getNickname();
	this->_hasNick = rhs.hasNick();
	this->_hasUser = rhs.hasUser();
	this->authenticated = rhs.isAuthenticated();
	return *this;
}

/* -------------------------------------------------------------------------- */
/*                                   getters                                  */
/* -------------------------------------------------------------------------- */

std::string Client::getUsername() const {
	return username;
}

std::string Client::getNickname() const {
	return nickname;
}

int Client::getFd() const {
	return fd;
}

socklen_t Client::getSize() const {
	return size;
}

bool Client::getStatus() const {
	return status;
}

sockaddr_storage Client::getAddr() const {
	return addr;
}

std::string Client::getTextAddr() const {
	return text_addr;
}

bool Client::isAuthenticated() const {
	return authenticated;
}

bool Client::hasUser() const {
	return (_hasUser);
}

bool Client::hasNick() const {
	return (_hasNick);
}

/* -------------------------------------------------------------------------- */
/*                                   setters                                  */
/* -------------------------------------------------------------------------- */

void Client::setUsername(std::string name) {
	this->username = name;
	std::cout << getFd() << ": ";
	error("CLIENT USERNAME", true);
}

void Client::setNickname(std::string name) {
	this->nickname = name;
	std::cout << getFd() << ": ";
	error("CLIENT NICKNAME", true);
}

void Client::setTextAddr(std::string addr) {
	this->text_addr = addr;
}

void Client::setAuthentication(bool status) {
 	this->authenticated = status;
	std::cout << getFd() << ": ";
	error("CLIENT AUTHENTICATION", status);
}

void Client::setStatus(bool status) {
	this->status = status;
}

void Client::setHasUser(bool status) {
	this->_hasUser = status;
}

void Client::setHasNick(bool status) {
	this->_hasNick = status;
}

/* -------------------------------------------------------------------------- */
/*                                   others                                   */
/* -------------------------------------------------------------------------- */

Client * Client::createClient(struct sockaddr_storage addr, socklen_t size, int fd) {
	return new Client(addr, size, fd);
}
