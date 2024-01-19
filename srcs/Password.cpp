# include "../includes/Password.hpp"

Password::Password(std::string pass) : password(pass) {}

Password::Password(Password const & src) {
	*this = src;
}

Password & Password::operator=(Password const & rhs) {
	this->password = rhs.getPassword();
	return *this;
}

Password::~Password() {}

std::string Password::getPassword() const {
	return password;
}

bool Password::validate(std::string pass) {
	return password.compare(pass) == 0;
}