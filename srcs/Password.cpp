# include "../includes/Password.hpp"


//? Can ew move all this things into the header file? its all so small that i dont see why not
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
	if (password.compare(0, sizeof(password), pass) == 0) {
		return true;
	}
	return false;
}