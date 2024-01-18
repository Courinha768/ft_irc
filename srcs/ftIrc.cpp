# include "../includes/ftIrc.hpp"

int main(int ac, char** av) {
	
	if (ac == 3) {

		Server server = Server(av[1], av[2]);

		server.setup();
		server.setupPoll();

	} else {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
	}

	return 0;

}