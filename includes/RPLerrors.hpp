#ifndef RPLERRORS_HPP
# define RPLERRORS_HPP

//trying to find the correct RPL reply
# define NEED_AUTHENTICATION	"FT_IRC Network: Type PASS to enter the servers password\n"

# define ERR_NOSUCHNICK(client, nickname)		(client + nickname + " :No such nick/channel\r\n")						//401
# define ERR_NOSUCHSERVER(client, servername)	(client + " " + servername + " :No such server\r\n")					//402
# define ERR_NOSUCHCHANNEL(client, servername)	(client + " " + servername + " :No such channel\r\n")					//403
# define ERR_CANNOTSENDTOCHA(client, channel)	(client + " " + channel + " :Cannot send to channel\r\n")				//404
# define ERR_TOOMANYCHANNELS(client, channel)	(client + " " + channel + " :You have joined too many channels\r\n")	//405
# define ERR_WASNOSUCHNICK(client)				(client + " :There was no such nickname\r\n")							//406
# define ERR_NOORIGIN(client)					(client + " :No origin specified\r\n")									//409
# define ERR_UNKNOWNCOMMAND(client, command)	(client + " " + command + " :Unknown command\r\n")						//421
# define ERR_NONICKNAMEGIVEN(client)			(client + " :No nickname given\r\n")									//431
# define ERR_ERRONEUSNICKNAME(client, nickname)	(client + " " + nickname + " :Erroneus nickname\r\n")					//432
# define ERR_NICKNAMEINUSE(client, nickname)	(client + " " + nickname + " :Nickname is already in use\r\n")			//433
# define ERR_NOTREGISTERED(nickname)			(nickname + " :You have not registered\r\n")							//451
# define ERR_NEEDMOREPARAMS(command)			(":ircserv.42.fr " + command + " :Not enough parameters\r\n")				//461
# define ERR_ALREADYREGISTERED(client)			(client + " :You may not reregister\r\n")								//462
# define ERR_PASSWDMISMATCH(client)				(client + " :Password incorrect\r\n")									//464

#endif