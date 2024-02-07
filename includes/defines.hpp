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

# define NEED_AUTHENTICATION	"FT_IRC Network: Type \\pass to enter the servers password\n"
# define NA_SIZE				57
# define NEED_REGISTRATION		"FT_IRC Network: Type \\nick to assign yourself a nickname\n"
# define NR_SIZE				57

# define PASS_COMMAND "\\pass"
# define USER_COMMAND "\\user"
# define NICK_COMMAND "\\nick"

# define WRONG_PASS_WARNING	"Wrong password!\r\n"
# define PASS_COMMAND_USAGE "Usage: \\pass <password>\r\n"

#endif
