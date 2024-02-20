#include "../../includes/ftIrc.hpp"

void Server::commandKICK(Client &client)
{
    //We have to change by the true confirmation that the customer is an operator
    bool isOperator = true;

    if (!isOperator)
    {
        sendWarning(NEED_CHANOPRIVS, client);
        return;
    }

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

    std::string channel_name;
    std::string user_to_kick;
    std::string comment;

    size_t end = message.find("\n");
    if (end == std::string::npos)
    {
        sendWarning(NEED_MOREPARAMS, client);
        return;
    }

    if (message.at(end - 1) == '\r')
        end = end - 1;

    std::string to_cut = message.substr(5, end - 5);
    end = to_cut.find(" ");
    if (end == std::string::npos)
    {
        sendWarning(NEED_MOREPARAMS, client);
        return;
    }
    channel_name = to_cut.substr(0, end);
    to_cut = to_cut.substr(end + 1);

    end = to_cut.find(" ");
    if (end != std::string::npos)
    {
        user_to_kick = to_cut.substr(0, end);
        comment = to_cut.substr(end + 1);
    }
    else
        user_to_kick = to_cut;

    bool channelFound = false;
    bool userFound = false;

    for (unsigned long i = 0; i < channels.size(); i++)
    {
        if (channels.at(i).getName() == channel_name)
        {
            channelFound = true;
            for (unsigned long j = 0; j < channels.at(i).getClients().size(); j++)
            {
                if (channels.at(i).getClients().at(j).getNickname() == user_to_kick)
                {
                    userFound = true;
                    std::string channel_notification = "Command to kick " + user_to_kick + " from " + channel_name;
                    if (!comment.empty()) {
                        channel_notification += " using \"" + comment + "\" as the reason (comment).";
                    }
                    channel_notification += "\r\n";
                    sendMessageToAllClients(channel_notification, client.getFd());

                    std::string client_notification = ":" + client.getNickname() + " kicked you from " + channel_name;
                    if (!comment.empty()) {
                        client_notification += " using \"" + comment + "\" as the reason (comment).";
                    }
                    client_notification += "\r\n";
                    sendMessageToClient(client_notification, channels.at(i).getClients().at(j).getFd());

                    channels.at(i).removeClient(channels.at(i).getClients().at(j));
                    break;
                }
            }
        }
    }

    if (!channelFound)
    {
        sendWarning(NOSUCHCHANNEL, client);
        return;
    }

    if (!userFound)
    {
        sendWarning(USERNOTINCHANNEL, client);
        return;
    }
}