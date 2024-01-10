# include "../includes/ftIrc.hpp"


int main(int ac, char** av) {
	
	if (ac == 3) {
		if (!Server::isPortValid(av[1])) {
			exit(1);
		}

		int port = atoi(av[1]);

		Server server = Server(port, av[2]);

		server.initialize_server();
	}

	return 0;

}