# include "../includes/ftIrc.hpp"
# include "../includes/Server.hpp"

int main(int ac, char** av) {
	
	if (ac == 3) {

		Server server = Server(av[1], av[2]);

		try
		{
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