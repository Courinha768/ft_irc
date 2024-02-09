#ifndef	DEFINES_HPP
# define DEFINES_HPP

# include <iostream>
# include <string>

# define BUFFER_SIZE 512

# define NUMERALS "0123456789"

# define INVALID_LEADING_CHARS "#&:"
# define WHITESPACES " \t\n\v\f\r"

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

# define PASS_COMMAND "PASS"
# define USER_COMMAND "USER"
# define NICK_COMMAND "NICK"

# define NEED_AUTHENTICATION	"FT_IRC Network: Type PASS to enter the servers password\n"
# define NEED_USER				"FT_IRC Network: Type USER to assign yourself a username\n"
# define NEED_REGISTRATION		"FT_IRC Network: You need to register before sending mensages\n"

# define COMMAND_NF	"FT_IRC Network: Command not found\n"

# define ERR_PASSWDMISMATCH ":ircserv.42.fr 464 :Password incorrect\r\n"
# define ERR_NICKNAMEINUSE ":ircserv.42.fr 433 :Nickname is already in use\r\n"
# define ERR_NONICKNAMEGIVEN ":ircserv.42.fr 431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nickname) (":ircserv.42.fr 432 " + nickname + " :Erroneus nickname\r\n")
# define PASS_COMMAND_USAGE "FT_IRC Network: Usage: PASS <password>\n"

# define ALREADY_AUTHENTICATED "FT_IRC Network: You are already authenticated\n"
# define ALREADY_USER "FT_IRC Network: You already have a username\n"

# define NL "\n"

#endif
