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
	"PASS", \
	"NICK", \
	"USER", \
	"JOIN", \
	"QUIT", \
	"MODE", \
	"PRIVMSG", \
	"KICK", \
	"INVITE", \
	"TOPIC", \
	"PART", \
}
# define MP_COMMAND_FUNCTIONS { \
	&Server::commandPASS, \
	&Server::commandNICK, \
	&Server::commandUSER, \
	&Server::commandJOIN, \
	&Server::commandQUIT, \
	&Server::commandMODE, \
	&Server::commandPRIVMSG, \
	&Server::commandKICK, \
	&Server::commandINVITE, \
	&Server::commandTOPIC, \
	&Server::commandPART, \
}
# define	MP_NOT_A_COMMAND -1


// End Of String
# define EOS std::string::npos
# define RP_MIN 1024
# define RP_MAX 65535
# define PORT_NOT_VALID 1
# define EPPOL_WAIT_ERROR -1

# define PASS_COMMAND "PASS"
# define USER_COMMAND "USER"
# define NICK_COMMAND "NICK"

# define NOUSER	"FT_IRC Network: That user doesn't exist"

#endif
