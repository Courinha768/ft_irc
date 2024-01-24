#include "../includes/Client.hpp"


Client::Client(struct sockaddr_storage addr, socklen_t size) : addr(addr), size(size) {
	authenticated = false;
}

Client::~Client() {}

Client::Client(Client const & src) {
	*this = src;
}

Client & Client::operator=(Client const & rhs) {
	this->addr = rhs.getAddr();
	this->size = rhs.getSize();
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

Client * Client::createClient(struct sockaddr_storage addr, socklen_t size) {
	return new Client(addr, size);
}

void Client::setStatus(bool status) {
	this->status = status;
}

bool Client::getStatus() const {
	return status;
}


