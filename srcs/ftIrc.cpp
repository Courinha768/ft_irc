# include "../includes/ftIrc.hpp"


int main(int ac, char** av) {
	
	if (ac == 3) {

		Server server = Server(atoi(av[1]), av[2]);

		server.initialize_server();
	}

	return 0;

}