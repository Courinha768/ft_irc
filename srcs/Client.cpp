#include "../includes/Client.hpp"


Client::Client(struct sockaddr_storage addr, socklen_t size, int fd) : addr(addr), size(size), fd(fd) {
	authenticated = false;
	registered = false;
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
	return *this;
}

std::string Client::getUsername() const {
	return username;
}

std::string Client::getNickname() const {
	return nickname;
}

void Client::setUsername(std::string name) {
	this->username = name;
}

void Client::setNickname(std::string name) {
	this->nickname = name;
}

sockaddr_storage Client::getAddr() const {
	return addr;
}

socklen_t Client::getSize() const {
	return size;
}
int Client::getFd() const {
	return fd;
}

std::string Client::getTextAddr() const {
	return text_addr;
}

void Client::setTextAddr(std::string addr) {
	this->text_addr = addr;
}
bool Client::isAuthenticated() const {
	return authenticated;
}

void Client::setAuthentication(bool status) {
 	authenticated = status;
	error("CLIENT AUTHENTICATION", status);
}

bool Client::isRegistered() const {
	if (!username.empty() && !nickname.empty()) return true;
	return false;
}

void Client::registration(bool status) {
	registered = status;
	error("REGISTRATION", status);
}

Client * Client::createClient(struct sockaddr_storage addr, socklen_t size, int fd) {
	return new Client(addr, size, fd);
}

void Client::setStatus(bool status) {
	this->status = status;
}

bool Client::getStatus() const {
	return status;
}


