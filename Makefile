NAME			= ircserv

SRCS			= $(wildcard srcs/*cpp) $(wildcard srcs/server/*cpp) $(wildcard srcs/commands/*cpp)
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

launch:			re
				@clear
				@./$(NAME) 8001 1

run:			$(NAME)
				@./$(NAME) 8000 1

.PHONY:			all clean fclean re