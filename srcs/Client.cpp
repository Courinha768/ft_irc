#include "../includes/Client.hpp"

Client::Client(struct sockaddr_storage addr, socklen_t size, int fd) : addr(addr), size(size), fd(fd) {
	authenticated = false;
	registered = false;
	text_addr = "";
	username = "";
	nickname = "";
	realname = "";
}

Client::~Client() {}

Client::Client(Client const & src) {
	*this = src;
}

Client & Client::operator=(Client const & rhs) {
	this->addr = rhs.getAddr();
	this->size = rhs.getSize();
	this->fd = rhs.getFd();
	this->text_addr = rhs.getTextAddr();
	this->username = rhs.getUsername();
	this->realname = rhs.getRealname();
	this->nickname = rhs.getNickname();
	this->registered = rhs.isRegistered();
	this->authenticated = rhs.isAuthenticated();
	this->status = rhs.getStatus();
	return *this;
}

/* -------------------------------------------------------------------------- */
/*                                   getters                                  */
/* -------------------------------------------------------------------------- */

std::string Client::getUsername() const {
	return username;
}

std::string Client::getRealname() const {
	return realname;
}

std::string Client::getNickname() const {
	return nickname;
}

bool Client::hasNick() const {
	return !nickname.empty();
}

bool Client::hasUser() const {
	return !username.empty();
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

bool Client::isRegistered() const {
	return (registered);
}

/* -------------------------------------------------------------------------- */
/*                                   setters                                  */
/* -------------------------------------------------------------------------- */

void Client::setUsername(std::string name) {
	this->username = name;
}

void Client::setRealname(std::string name) {
	this->realname = name;
}

void Client::setNickname(std::string name) {
	this->nickname = name.substr(0,9);
}

void Client::setTextAddr(std::string addr) {
	this->text_addr = addr;
}

void Client::setAuthentication(bool status) {
 	this->authenticated = status;
}

void Client::setStatus(bool status) {
	this->status = status;
}

void Client::setisRegistered(bool status) {
	this->registered = status;
}

/* -------------------------------------------------------------------------- */
/*                                   others                                   */
/* -------------------------------------------------------------------------- */

Client * Client::createClient(struct sockaddr_storage addr, socklen_t size, int fd) {
	return new Client(addr, size, fd);
}