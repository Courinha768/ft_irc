# include "../includes/Password.hpp"

size_t	stringVectorLenght(std::string vector[]) {
	size_t	i = -1;

	while (!vector[++i].empty()) {}
	return (i);
}