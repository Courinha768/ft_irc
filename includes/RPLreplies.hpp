#ifndef RPLREPLIES_HPP
# define RPLREPLIES_HPP

#define RPL_WELCOME(nickname)								(":ircserv.42.fr 001 " + nickname + " :Welcome to the FT_IRC Network, " + nickname)			//001
#define RPL_YOURHOST(nickname)								(":ircserv.42.fr 002 " + nickname + " :Your host is ircserv.42.fr running version 001.1")	//002
#define RPL_CREATED(nickname)								(":ircserv.42.fr 003 " + nickname + " :This server was created on thu Feb 01 2024")			//003
#define RPL_MYINFO(nickname)								(":ircserv.42.fr 004 " + nickname + " :ircserv.42.fr 001.1")								//004

#define RPL_TOPIC(nickname, channel_name, channel_topic)	(":ircserv.42.fr 332 " + nickname + " "	+ channel_name " :" + channel_topic)
#define RPL_NAMREPLY(nickname, channel_name)				(":ircserv.42.fr 353 " + nickname + " = " + channel_name ":@" + nickname)
#define RPL_ENDOFNAMES(nickname, channel_name)				(":ircserv.42.fr 366 " + nickname + " "	+ channel_name " :End of /NAMES list.")


#endif