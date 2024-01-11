#include "../includes/Client.hpp"


Client::Client(struct sockaddr_storage addr, socklen_t size) : addr(addr), size(size) {}

Client::~Client() {}

Client::Client(Client const & src) {
	*this = src;
}

Client & Client::operator=(Client const & rhs) {
	this->addr = rhs.getAddr();
	this->size = rhs.getSize();
	return *this;
}


sockaddr_storage Client::getAddr() const {
	return addr;
}

socklen_t Client::getSize() const {
	return size;
}

void Client::setSize(socklen_t size) {
	this->size = size;
}

Client * Client::createClient(struct sockaddr_storage addr, socklen_t size) {
	return new Client(addr, size);
}