#ifndef RPLREPLIES_HPP
# define RPLREPLIES_HPP

#define RPL_WELCOME(nick)								":ircserv.42.fr 001 " + nick + " :Welcome to the FT_IRC Network, " + nick				//001
#define RPL_YOURHOST(nick)								":ircserv.42.fr 002 " + nick + " :Your host is ircserv.42.fr running version 001.1"		//002
#define RPL_CREATED(nick)								":ircserv.42.fr 003 " + nick + " :This server was created on thu Feb 01 2024"			//003
#define RPL_MYINFO(nick)								":ircserv.42.fr 004 " + nick + " :ircserv.42.fr 001.1"									//004

#define RPL_TOPIC(nick, channel, topic)					":ircserv.42.fr 332 " + nick + " "	+ channel + " :" + topic							//332
#define RPL_TOPICWHOTIME(nick, channel, topic, setat)	":ircserv.42.fr 333 " + nick + " "	+ channel + " " + nick + " " + setat				//333
#define RPL_INVITING(nick, channel)						":ircserv.42.fr 341 " + nick + " " + channel + "\r\n"									//341
#define RPL_NAMREPLY(nick, channel)						":ircserv.42.fr 353 " + nick + " = " + channel + ":@" + nick							//353
#define RPL_ENDOFNAMES(nick, channel)					":ircserv.42.fr 366 " + nick + " "	+ channel + ":End of /NAMES list."					//366

#define RPL_CHANNELMODEIS(nick, channel, modes)				":ircserv.42.fr 366 " + nick + " "	+ channel + " " + modes								//324

#define JOIN_REPLY(nick, channel)						":" + nick + "!" + nick + "@ircserv.42.fr JOIN " + channel

#endif