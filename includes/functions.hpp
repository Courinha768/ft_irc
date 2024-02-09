#ifndef	FUNCTIONS_HPP
# define FUNCTIONS_HPP

# include "ftIrc.hpp"
# include "Server.hpp"

void	error(std::string service, bool status);
size_t	stringVectorLenght(std::string vector[]);

/* -------------------------------- COMMANDS -------------------------------- */
void commandCAP(Client & client, Server & server);
void commandJOIN(Client & client, Server & server);
void commandNICK(Client & client, Server & server);
void commandPASS(Client & client, Server & server);
void commandQUIT(Client & client, Server & server);
void commandUSER(Client & client, Server & server);

#endif
