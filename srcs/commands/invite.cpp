#include "../../includes/ftIrc.hpp"

void Server::commandINVITE(Client & client)
{
    if (!client.isAuthenticated())
    {
        sendWarning(NEED_AUTHENTICATION, client);
        return;
    }

    if (!client.isRegistered())
    {
        sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
        return;
    }

    std::string nickname_to_invite;
    std::string channel_name;

    size_t end = message.find("\n");
    if (end == std::string::npos)
    {
        sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 6)), client);
        return;
    }

    if (message.at(end - 1) == '\r')
        end = end - 1;

    std::string to_cut = message.substr(8, end - 8);
    end = to_cut.find(" ");
    if (end == std::string::npos)
    {
        sendWarning(ERR_NEEDMOREPARAMS(message.substr(0, 6)), client);
        return;
    }
    nickname_to_invite = to_cut.substr(0, end);
    to_cut = to_cut.substr(end + 1);

    end = to_cut.find(" ");
    if (end == std::string::npos)
    {
        channel_name = to_cut;
    }
    else
    {
        channel_name = to_cut.substr(0, end);
    }

    Channel* target_channel = findChannelByName(channel_name);
    if (target_channel == NULL)
    {
        sendWarning(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name), client);
        return;
    }

    bool isMember = false;
    for (size_t i = 0; i < target_channel->getClients().size(); ++i)
    {
        if (target_channel->getClients()[i].getNickname() == client.getNickname())
        {
            isMember = true;
            break;
        }
    }

    if (!isMember)
    {
        sendWarning(ERR_NOTONCHANNEL(client.getNickname(), channel_name), client);
        return;
    }

    bool isAlreadyInChannel = false;
    for (size_t i = 0; i < target_channel->getClients().size(); ++i)
    {
        if (target_channel->getClients()[i].getNickname() == nickname_to_invite)
        {
            isAlreadyInChannel = true;
            break;
        }
    }

    if (isAlreadyInChannel)
    {
        sendWarning(ERR_USERONCHANNEL(client.getNickname(), nickname_to_invite, channel_name), client);
        return;
    }

   for (size_t i = 0; i < clients.size(); ++i)
    {
        if (clients[i]->getNickname() == nickname_to_invite)
        {
            std::string invite_notification = "Invite " + nickname_to_invite + " to " + channel_name + "\r\n";
            sendMessageToClient(invite_notification, clients[i]->getFd());
            RPL_INVITING(client.getNickname(), client.getFd(), nickname_to_invite, channel_name);
            return;
        }
    }

    sendWarning(NOUSER, client);
}