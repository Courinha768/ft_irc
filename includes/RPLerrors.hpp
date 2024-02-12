#ifndef RPLERRORS_HPP
# define RPLERRORS_HPP

//trying to find the correct RPL reply
# define NEED_AUTHENTICATION	"FT_IRC Network: Type PASS to enter the servers password\n"

# define ERR_NOSUCHNICK(client, nickname)		(":ircserv.42.fr 401 " + client + nickname + " :No such nick/channel\r\n")						//401
# define ERR_NOSUCHSERVER(client, servername)	(":ircserv.42.fr 402 " + client + " " + servername + " :No such server\r\n")					//402
# define ERR_NOSUCHCHANNEL(client, servername)	(":ircserv.42.fr 403 " + client + " " + servername + " :No such channel\r\n")					//403
# define ERR_CANNOTSENDTOCHA(client, channel)	(":ircserv.42.fr 404 " + client + " " + channel + " :Cannot send to channel\r\n")				//404
# define ERR_TOOMANYCHANNELS(client, channel)	(":ircserv.42.fr 405 " + client + " " + channel + " :You have joined too many channels\r\n")	//405
# define ERR_WASNOSUCHNICK(client)				(":ircserv.42.fr 406 " + client + " :There was no such nickname\r\n")							//406
# define ERR_NOORIGIN(client)					(":ircserv.42.fr 409 " + client + " :No origin specified\r\n")									//409
# define ERR_UNKNOWNCOMMAND(client, command)	(":ircserv.42.fr 421 " + client + " " + command + " :Unknown command\r\n")						//421
# define ERR_NONICKNAMEGIVEN(client)			(":ircserv.42.fr 431 " + client + " :No nickname given\r\n")									//431
# define ERR_ERRONEUSNICKNAME(client, nickname)	(":ircserv.42.fr 432 " + client + " " + nickname + " :Erroneus nickname\r\n")					//432
# define ERR_NICKNAMEINUSE(client, nickname)	(":ircserv.42.fr 433 " + client + " " + nickname + " :Nickname is already in use\r\n")			//433
# define ERR_NOTREGISTERED(nickname)			(":ircserv.42.fr 451 " + nickname + " :You have not registered\r\n")							//451
# define ERR_NEEDMOREPARAMS(command)			(":ircserv.42.fr 461 " + command + " :Not enough parameters\r\n")								//461
# define ERR_ALREADYREGISTERED(client)			(":ircserv.42.fr 462 " + client + " :You may not reregister\r\n")								//462
# define ERR_PASSWDMISMATCH(client)				(":ircserv.42.fr 464 " + client + " :Password incorrect\r\n")									//464
# define ERR_BADCHANNELKEY(client, channel)		(":ircserv.42.fr 475 " + client + " " + channel + " :Password incorrect\r\n")					//475

#endif