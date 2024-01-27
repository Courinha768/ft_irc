#include "../includes/ftIrc.hpp"

/**
 * Displays the status of a service operation.
 *
 * @param service The name of the service.
 * @param status The status of the service operation.
 */
void	error(std::string service, bool status)
{
	std::cout << service << ":";
	for (int i = 1; i < (25 - (int)service.length());  i++) {
		std::cout << " "; }
	if (status)
		std::cout << BGRN << "OK" << CRESET << std::endl;
	else
		std::cout << BRED << "ERROR" << CRESET << std::endl;
}