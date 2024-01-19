#include "../includes/Client.hpp"


Client::Client(struct sockaddr_storage addr, socklen_t size) : addr(addr), size(size) {}

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

Client * Client::createClient(struct sockaddr_storage addr, socklen_t size) {
	return new Client(addr, size);
}

void Client::handleCommunication(int fd, bool * connectionUp) {
	int data;
	memset(message, 0, BUFFER_SIZE);

	std::cout << "waiting for data" << std::endl;
	if (read(fd, message, BUFFER_SIZE) >= 0) {
		memcpy(&data, message, sizeof(int));
		std::cout << "message: " << message << std::endl;
	}
	if (data == 0) {
		*connectionUp = false;
		std::cout << "coonection lost with client " << getTextAddr() << std::endl;
		memset(message, 0, BUFFER_SIZE);
		write(fd, message, BUFFER_SIZE);
	}

}
