#ifndef	DEFINES_HPP
# define DEFINES_HPP

# define NETWORK ":ircserv.42.fr"

# define BUFFER_SIZE 512
# define NL "\n"
# define NUMERALS "0123456789"

# define INVALID_LEADING_CHARS "#&:"
# define WHITESPACES " \t\n\v\f\r"


/* ----------------------------- MessageParsing ----------------------------- */
# define MP_AVAILABLE_COMMANDS { \
	"CAP", \
	"PASS", \
	"NICK", \
	"USER", \
	"JOIN", \
	"QUIT", \
	"MODE", \
	"PRIVMSG", \
	"KICK", \
}
# define MP_COMMAND_FUNCTIONS { \
	&Server::commandCAP, \
	&Server::commandPASS, \
	&Server::commandNICK, \
	&Server::commandUSER, \
	&Server::commandJOIN, \
	&Server::commandQUIT, \
	&Server::commandMODE, \
	&Server::commandPRIVMSG, \
	&Server::commandKICK, \
}
# define MP_NOT_A_COMMAND -1


// End Of String
# define EOS std::string::npos
# define RP_MIN 1024
# define RP_MAX 65535
# define PORT_NOT_VALID 1
# define EPPOL_WAIT_ERROR -1

# define PASS_COMMAND "PASS"
# define USER_COMMAND "USER"
# define NICK_COMMAND "NICK"

# define NEED_AUTHENTICATION	"FT_IRC Network: Type PASS to enter the servers password\n"
# define NEED_USER				"FT_IRC Network: Type USER to assign yourself a username\n"
# define NEED_REGISTRATION		"FT_IRC Network: You need to register before sending mensages\n"
# define NEED_CHANOPRIVS   		"FT_IRC Network: KICK: You're not channel operator\n"
# define NEED_MOREPARAMS		"FT_IRC Network: KICK: Not enough parameters\n"
# define NOSUCHCHANNEL			"FT_IRC Network: KICK: No such channel\n"
# define USERNOTINCHANNEL		"FT_IRC Network: KICK: That user is not on that channel\n"

# define COMMAND_NF	"FT_IRC Network: Command not found\n"

# define PASS_COMMAND_USAGE "FT_IRC Network: Usage: PASS <password>\n"

# define ALREADY_AUTHENTICATED "FT_IRC Network: You are already authenticated\n"
# define ALREADY_USER "FT_IRC Network: You already have a username\n"

# define NL "\n"

#endif
