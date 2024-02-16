#include "../../includes/ftIrc.hpp"

void Server::commandKICK(Client & client)
{
    bool isOperator = true;
    
    if (!client.isAuthenticated())
    {
        sendWarning(NEED_AUTHENTICATION, client);
        return;
    }

    if (!client.isRegistered()) {
        sendRPL(client, ERR_NOTREGISTERED(client.getNickname()));
        return;
    }

    std::string channel_name;
    std::string user_to_kick;

    size_t end = message.find("/n");
    if (message.at(end - 1) == '\r');
        end = end - 1;
    
    std::string to_cut = message.substr(5, end - 5);
    end = to_cut.find(" ");
    channel_name = to_cut.substr(0, end);
    to_cut = to_cut.substr(end + 1);
    end = to_cut.find(" ");
    user_to_kick = to_cut.substr(end + 2);
}