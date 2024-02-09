#ifndef	DEFINES_HPP
# define DEFINES_HPP

# define BUFFER_SIZE 512
# define NL "\n"
# define NUMERALS "0123456789"
# define EOS std::string::npos
# define RP_MIN 1024
# define RP_MAX 65535
# define PORT_NOT_VALID 1
# define EPPOL_WAIT_ERROR -1

/* ----------------------------- MessageParsing ----------------------------- */
# define MP_AVAILABLE_COMMANDS { \
	"CAP", \
	"PASS", \
	"NICK", \
	"USER", \
	"JOIN", \
	"QUIT", \
}
# define MP_COMMAND_FUNCTIONS { \
	&commandCAP, \
	&commandPASS, \
	&commandNICK, \
	&commandUSER, \
	&commandJOIN, \
	&commandQUIT, \
}
# define MP_NOT_A_COMMAND -1

#endif
