#ifndef _PASSWORD_HPP_
# define _PASSWORD_HPP_

#include "ftIrc.hpp"

class Password {

	private:
		std::string password;
	
	public:
		Password(std::string pass);
		~Password();
		Password(Password const & src);
		Password & operator=(Password const & rhs);
		bool validate(std::string pass);
		std::string getPassword() const;

};

#endif