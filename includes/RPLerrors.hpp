#ifndef RPLERRORS_HPP
# define RPLERRORS_HPP

//trying to find the correct RPL reply
# define NEED_AUTHENTICATION	"FT_IRC Network: Type PASS to enter the servers password\n"

# define ERR_NOSUCHNICK(client, nickname)					(":ircserv.42.fr 401 " + client + nickname + " :No such nick/channel")										//401
# define ERR_NOSUCHSERVER(client, servername)				(":ircserv.42.fr 402 " + client + " " + servername + " :No such server")									//402
# define ERR_NOSUCHCHANNEL(client, channel)					(":ircserv.42.fr 403 " + client + " " + channel + " :No such channel")										//403
# define ERR_CANNOTSENDTOCHA(client, channel)				(":ircserv.42.fr 404 " + client + " " + channel + " :Cannot send to channel")								//404
# define ERR_TOOMANYCHANNELS(client, channel)				(":ircserv.42.fr 405 " + client + " " + channel + " :You have joined too many channels")					//405
# define ERR_WASNOSUCHNICK(client)							(":ircserv.42.fr 406 " + client + " :There was no such nickname")											//406
# define ERR_NOORIGIN(client)								(":ircserv.42.fr 409 " + client + " :No origin specified")													//409
# define ERR_UNKNOWNCOMMAND(client, command)				(":ircserv.42.fr 421 " + client + " " + command + " :Unknown command")										//421
# define ERR_NONICKNAMEGIVEN(client)						(":ircserv.42.fr 431 " + client + " :No nickname given")													//431
# define ERR_ERRONEUSNICKNAME(client, nickname)				(":ircserv.42.fr 432 " + client + " " + nickname + " :Erroneus nickname")									//432
# define ERR_NICKNAMEINUSE(client, nickname)				(":ircserv.42.fr 433 " + client + " " + nickname + " :Nickname is already in use")							//433
# define ERR_USERNOTINCHANNEL(client, nickname, channel)	(":ircserv.42.fr 441 " + client + " " + nickname + " " + channel + " :That user is not on that channel")	//441
# define ERR_NOTONCHANNEL(client, channel)					(":ircserv.42.fr 442 " + client + " " + channel + " :You're not on that channel")							//442
# define ERR_USERONCHANNEL(client, nickname, channel)		(":ircserv.42.fr 443 " + client + " " + nickname + " " + channel + " :is already on channel")				//443
# define ERR_NOTREGISTERED(nickname)						(":ircserv.42.fr 451 " + nickname + " :You have not registered")											//451
# define ERR_NEEDMOREPARAMS(channel, nick, command)			(":ircserv.42.fr 461 " + nick + " " + channel + " " + command + " :Not enough parameters")												//461
# define ERR_ALREADYREGISTERED(client)						(":ircserv.42.fr 462 " + client + " :You may not reregister")												//462
# define ERR_PASSWDMISMATCH(client)							(":ircserv.42.fr 464 " + client + " :Password incorrect")													//464
# define ERR_BADCHANNELKEY(client, channel)					(":ircserv.42.fr 475 " + client + " " + channel + " :Password incorrect")									//475
# define ERR_CHANOPRIVSNEEDED(client, channel)				(":ircserv.42.fr 482 " + client + " " + channel + " :You're not channel operator")							//482
# define ERR_UMODEUNKNOWNFLAG(client)						(":ircserv.42.fr 501 " + client + " :Unknown MODE flag")													//501
# define ERR_INVITEONLYCHAN(client, channel) 				(":ircserv.42.fr 473 " + client + " " + channel + " :Cannot join channel (+i)")									//473
# define ERR_CHANNELISFULL(client, channel) 				(":ircserv.42.fr 471 " + client + " " + channel + " :Cannot join channel (+l)")									//471

#endif