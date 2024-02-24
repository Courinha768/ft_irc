# include "../includes/Password.hpp"

size_t	stringVectorLenght(std::string vector[]) {
	size_t	i = -1;

	while (!vector[++i].empty()) {}
	return (i);
}

void	error(std::string service, bool status)
{
	std::cout << service << ":";
	for (int i = 1; i < (25 - (int)service.length());  i++) {
		std::cout << " "; }
	if (status)
		std::cout << BGRN << "OK" << CRESET << std::endl;
	else
		std::cout << BRED << "ERROR" << CRESET << std::endl;
}

void sendMsg(Client & client, std::string message) {
	message = message + "\r\n";
	send(client.getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
}

