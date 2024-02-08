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

//It works with both PASS and \pass
# define PASS_COMMAND1 "\\pass"
# define USER_COMMAND1 "\\user"
# define NICK_COMMAND1 "\\nick"
# define PASS_COMMAND2 "PASS"
# define USER_COMMAND2 "USER"
# define NICK_COMMAND2 "NICK"

# define NEED_AUTHENTICATION	"FT_IRC Network: Type \\pass to enter the servers password\n"
# define NEED_USER				"FT_IRC Network: Type \\user to assign yourself a username\n"
# define NEED_REGISTRATION		"FT_IRC Network: You need to register before sending mensages\n"

# define COMMAND_NF	"FT_IRC Network: Command not found\n"

# define WRONG_PASS_WARNING	"FT_IRC Network: Wrong password!\n"
# define PASS_COMMAND_USAGE "FT_IRC Network: Usage: \\pass <password>\n"

# define ALREADY_AUTHENTICATED "FT_IRC Network: You are already authenticated\n"
# define ALREADY_USER "FT_IRC Network: You already have a nickname\n"

# define NL "\n"

#endif
