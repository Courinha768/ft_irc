#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include "ftIrc.hpp"

class Client;
class Channel;
class Password;

class Server {

	private:

		std::string				port;
		Password				*password;

		struct addrinfo			serv;
		struct addrinfo			*servinfo;

		std::map<int, Client *>	clients;
		char					recv_buffer[BUFFER_SIZE];
		std::string				message;

		struct	epoll_event 	events[200];
		struct	epoll_event 	event;
		
		int						eventsCount;
		int						status;
		int						sockfd;
		int						efd;

		std::vector<Channel>	channels;

		
		
		

	public:

		Server(std::string port, std::string password);
		~Server();
		
		in_addr get_in_addr(struct sockaddr *sa);

		void setup();
		void acceptNewClient();
		void setupPoll();
		void receiveMessage(Client & client);
		void parseMessage(Client & client);
		void sendRPLwellcome(Client & client);
		void sendRPL(Client & client, std::string message);
		bool isMsgEmpty(std::string msg);
		bool hasInvalidChars(std::string msg);
		void sendMessageToAllClients(std::string msg, int client_fd);
		void sendMessageToClient(std::string msg, int client_fd);
		void sendMsgToAllClientsOnChannel(std::string channel_name, std::string msg);
		Channel & findChannelByName(const std::string& name);
		Client* findClientByNickname(const std::string& nickname);
		bool isClientOnChannel(std::string client_nickname, std::string channel_name);
		bool isClientOnServer(std::string client_nickname);

		void commandJOIN(Client & client);
		void commandNICK(Client & client);
		void commandPASS(Client & client);
		void commandQUIT(Client & client);
		void commandUSER(Client & client);
		void commandMODE(Client & client);
		void commandPRIVMSG(Client & client);
		void commandKICK(Client & client);
		void commandINVITE(Client & client);
		void commandTOPIC(Client & client);
		void commandPART(Client & client);
		

		template<typename T>
		Server& operator<<(const T& data) {
			std::cout << data;
			return *this;
		}
		Server& cout() {
			return *this;
		}

		class ChannelNotFoundException : public std::exception {

			public:
				virtual const char * what() const throw() {
					return "Channel not found!";
				}
		};

};


#endif