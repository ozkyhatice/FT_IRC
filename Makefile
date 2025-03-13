NAME = ircserv

SRCS = 	main.cpp Server.cpp Client.cpp Channel.cpp Commands/Help.cpp Commands/Nick.cpp Commands/User.cpp Commands/Pass.cpp Commands/Privmsg.cpp Commands/Join.cpp Commands/Topic.cpp Commands/Kick.cpp Commands/Mode.cpp Commands/Invite.cpp development/utils.cpp

OBJS = $(SRCS:.cpp=.o)
CXX_STANDARD = c++98
CXX = c++
CXXFLAGS = -g -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME) .vscode

re: fclean all

# Valgrind target to check for memory leaks
valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

.PHONY: all clean fclean re valgrind
