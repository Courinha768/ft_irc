# include "../includes/ftIrc.hpp"

// ! Branch Courinha
int main(int ac, char** av) {
	
	if (ac == 3) {

		Server server = Server(av[1], av[2]);

		server.initialize_server();
	} else {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
	}

	return 0;

}