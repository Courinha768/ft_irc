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
		void authenticate(Client & client);
		void sendWarning(std::string msg, Client & client);
		void setClientUser(Client & client);
		void setClientNick(Client & client);
		void sendRPLwellcome(Client & client);
		void sendRPL(Client & client, std::string message);
		bool isMsgEmpty(std::string msg);
		bool hasInvalidChars(std::string msg);
		void sendMessageToAllClients(std::string msg, int client_fd);

		void commandCAP(Client & client);
		void commandJOIN(Client & client);
		void commandNICK(Client & client);
		void commandPASS(Client & client);
		void commandQUIT(Client & client);
		void commandUSER(Client & client);
		void commandMODE(Client & client);
		void commandPRIVMSG(Client & client);
		void commandKICK(Client & client);
		

		template<typename T>
		Server& operator<<(const T& data) {
			std::cout << data;
			return *this;
		}
		Server& cout() {
			return *this;
		}

};


#endif