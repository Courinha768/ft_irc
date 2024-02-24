# include "../includes/ftIrc.hpp"
# include "../includes/Server.hpp"

void signalHandler(int sig) {
	(void)sig;
	throw std::runtime_error("Exiting.");
}

int main(int ac, char** av) {
	
	if (ac == 3) {

		Server server = Server(av[1], av[2]);


		try
		{
			signal(SIGINT, signalHandler);
			signal(SIGTSTP, signalHandler);
			signal(SIGTERM, signalHandler);
			signal(SIGQUIT, signalHandler);
			server.setup();
			server.setupPoll();
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
		

	} else {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
	}

	return 0;

}