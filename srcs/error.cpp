#include "../includes/ftIrc.hpp"

void	error(std::string service, bool status)
{
	std::cout << service << ":";
	for (int i = 1; i < (17 - (int)service.length());  i++) {
		std::cout << " "; }
	if (status)
		std::cout << BGRN << "OK" << CRESET << std::endl;
	else
		std::cout << BRED << "ERROR" << CRESET << std::endl;
}