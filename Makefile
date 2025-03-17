NAME = ircserv

SRCS = 	main.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		Commands/Help.cpp \
		Commands/Nick.cpp \
		Commands/User.cpp \
		Commands/Pass.cpp \
		Commands/Privmsg.cpp \
		Commands/Join.cpp \
		Commands/Topic.cpp \
		Commands/Kick.cpp \
		Commands/Mode.cpp \
		Commands/Invite.cpp

OBJS = $(SRCS:.cpp=.o)

CXX = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
