#ifndef	FUNCTIONS_HPP
# define FUNCTIONS_HPP

# include "ftIrc.hpp"
# include "Server.hpp"

void	error(std::string service, bool status);
size_t	stringVectorLenght(std::string vector[]);

void sendMsg(Client & client, std::string message);



#endif
