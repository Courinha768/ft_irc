#ifndef	DEFINES_HPP
# define DEFINES_HPP

# define BUFFER_SIZE 512

# define NUMERALS "0123456789"

// End Of String
# define EOS std::string::npos

// Registered Ports
# define RP_MIN 1024
# define RP_MAX 65535

// Exit codes
# define PORT_NOT_VALID 1

// Replies - RPL
#define RPL_WELCOME(nickname)	(":ircserv.42.fr 001 " + nickname + " :Welcome to the FT_IRC Network, " + nickname + "\r\n")
#define RPL_YOURHOST(nickname)	(":ircserv.42.fr 002 " + nickname + " :Your host is ircserv.42.fr running version 001.1\r\n")
#define RPL_CREATED(nickname)	(":ircserv.42.fr 003 " + nickname + " :This server was created on thu Feb 01 2024\r\n")
#define RPL_MYINFO(nickname)	(":ircserv.42.fr 004 " + nickname + " :ircserv.42.fr 001.1\r\n")

#define RPL_TOPIC(nickname, channel_name, channel_topic)	(":ircserv.42.fr 332 " + nickname + " #"	+ channel_name ":" + channel_topic + "\r\n")
#define RPL_NAMREPLY(nickname, channel_name)	(":ircserv.42.fr 353 " + nickname + " = "	+ channel_name ":@" + nickname + "\r\n")
#define RPL_ENDOFNAMES(nickname, channel_name)	(":ircserv.42.fr 366 " + nickname + " "	+ channel_name " :End of /NAMES list.\r\n")

# define EPPOL_WAIT_ERROR -1

// MessageParsing
# define MP_AVAILABLE_COMMANDS { \
	"CAP", \
	"PASS", \
	"NICK", \
	"USER", \
	"JOIN", \
	"QUIT", \
}
# define MP_COMMAND_FUNCTIONS { \
	&commandCAP, \
	&commandPASS, \
	&commandNICK, \
	&commandUSER, \
	&commandJOIN, \
	&commandQUIT, \
}
# define MP_NOT_A_COMMAND -1





# define NEED_AUTHENTICATION	"FT_IRC Network: Type PASS to enter the servers password\n"
# define NEED_USER				"FT_IRC Network: Type USER to assign yourself a username\n"
# define ERR_NOTREGISTERED(nickname) nickname + " :You have not registered"

# define COMMAND_NF	"FT_IRC Network: Command not found\n"

# define WRONG_PASS_WARNING	"FT_IRC Network: Wrong password!\n"
# define PASS_COMMAND_USAGE "FT_IRC Network: Usage: PASS <password>\n"

# define ALREADY_AUTHENTICATED "FT_IRC Network: You are already authenticated\n"
# define ALREADY_USER "FT_IRC Network: You already have a username\n"

# define NL "\n"

#endif
