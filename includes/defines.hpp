#ifndef	DEFINES_HPP
# define DEFINES_HPP

# include <iostream>
# include <string>

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
# define EPPOL_WAIT_ERROR -1

# define NEED_AUTHENTICATION	"something\n"
# define NA_SIZE				11
# define NEED_REGISTRATION		"something else\n"
# define NR_SIZE				16

#endif
