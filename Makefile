NAME			= ircserv

SRCS			= ./srcs/ftIrc.cpp ./srcs/Server.cpp ./srcs/Client.cpp ./srcs/Password.cpp
OBJS			= $(SRCS:.cpp=.o)

INC				= -I ./includes

CXX				= c++
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98

REMOVE			= rm

all:			$(NAME)

$(NAME):		$(OBJS)
				@$(CXX) $(CXXFLAGS) $(INC) -o $(NAME) $(OBJS)

clean:
				@rm -rf $(OBJS)
				@echo "\e[1mObject files deleted\033[0m"

fclean:			clean
				@rm -rf $(NAME)
				@echo "\e[1mExecutable files deleted\033[0m"

re:				fclean $(NAME)

.PHONY:			all clean fclean re