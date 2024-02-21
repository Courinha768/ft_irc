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
	"INVITE", \
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
	&Server::commandINVITE, \
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

# define NOUSER	"FT_IRC Network: That user doesn't exist\r\n"

# define COMMAND_NF	"FT_IRC Network: Command not found\r\n"

# define PASS_COMMAND_USAGE "FT_IRC Network: Usage: PASS <password>\r\n"

# define ALREADY_AUTHENTICATED "FT_IRC Network: You are already authenticated\r\n"
# define ALREADY_USER "FT_IRC Network: You already have a username\r\n"

# define NL "\n"

#endif
