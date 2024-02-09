#ifndef RPLERRORS_HPP
# define RPLERRORS_HPP

# define NEED_AUTHENTICATION	"FT_IRC Network: Type PASS to enter the servers password\n"
# define NEED_USER				"FT_IRC Network: Type USER to assign yourself a username\n"
# define ERR_NOTREGISTERED(nickname) nickname + " :You have not registered"

# define COMMAND_NF	"FT_IRC Network: Command not found\n"

# define WRONG_PASS_WARNING	"FT_IRC Network: Wrong password!\n"
# define PASS_COMMAND_USAGE "FT_IRC Network: Usage: PASS <password>\n"

# define ALREADY_AUTHENTICATED "FT_IRC Network: You are already authenticated\n"
# define ALREADY_USER "FT_IRC Network: You already have a username\n"


# define ERR_NOSUCHNICK(client, nickname)		client + " " + nickname + " :No such nick/channel"				//401
# define ERR_NOSUCHSERVER(client, servername)	client + " " + servername + " :No such server"					//402
# define ERR_NOSUCHCHANNEL(client, servername)	client + " " + servername + " :No such channel"					//403
# define ERR_CANNOTSENDTOCHA(client, channel)	client + " " + channel + " :Cannot send to channel"				//404
# define ERR_TOOMANYCHANNELS(client, channel)	client + " " + channel + " :You have joined too many channels"	//405
# define ERR_WASNOSUCHNICK(client)				client + " :There was no such nickname"							//406
# define ERR_NOORIGIN(client)					client + " :No origin specified"								//409
# define ERR_UNKNOWNCOMMAND(client, command)	client + " " + command + " :Unknown command"					//421
# define ERR_NONICKNAMEGIVEN(client)			client + " :No nickname given"									//431
# define ERR_ERRONEUSNICKNAME(client, nick)		client + " " + nick + " :Erroneus nickname"						//432
# define ERR_NICKNAMEINUSE(client, nick)		client + " " + nick + " :Nickname is already in use"			//433
# define ERR_NEEDMOREPARAMS(client, command)	client + " " + command + " :Not enough parameters"				//461
# define ERR_ALREADYREGISTERED(client)			client + " :You may not reregister"								//462
# define ERR_PASSWDMISMATCH(client)				client + " :Password incorrect"									//464

#endif