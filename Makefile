# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/15 15:11:41 by bsilva-c          #+#    #+#              #
#    Updated: 2024/01/20 19:07:32 by bsilva-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET	= \033[0m
BLACK 	= \033[1;30m
RED 	= \033[1;31m
GREEN 	= \033[1;32m
YELLOW 	= \033[1;33m
BLUE	= \033[1;34m
PURPLE 	= \033[1;35m
CYAN 	= \033[1;36m
WHITE 	= \033[1;37m

CXX = c++
RM = rm -rf
AR = ar -rcs

CPPFLAGS	= -Wall -Wextra -Werror -g -std=c++98
MKFLAGS		= --no-print-directory

DEPS			= ./includes
SRCS			= ./sources
VPATH			= $(SRCS) $(addprefix $(SRCS)/, $(_SUBFOLDERS))
OBJECTS			= objects

NAME			= webserv
FILES			= main Cluster Server Location CommonDirectives Request Response Cgi utils Connection
OBJS			= $(FILES:%=%.o)
TARGET			= $(addprefix $(OBJECTS)/, $(OBJS))

all: $(NAME)

$(NAME): $(OBJECTS) $(TARGET)
	$(CXX) $(CPPFLAGS) $(TARGET) -o $(NAME) -I $(DEPS)
	echo "[$(GREEN)Program name$(RESET)] $(NAME)"
	echo "[$(GREEN)Done$(RESET)]"

$(OBJECTS)/%.o : %.cpp
	echo "[$(CYAN)Compiling$(RESET)] $(CPPFLAGS) $(GREEN)$<$(RESET)"
	$(CXX) $(CPPFLAGS) -c $< -o $@ -I $(DEPS)

$(OBJECTS):
	mkdir -p $(OBJECTS)

clean:
	echo "[$(RED)cleaned$(RESET)]"
	$(RM) $(OBJECTS)
	$(RM) -rf $(OBJECTS)

fclean: clean
	echo "[$(RED)cleaned even better$(RESET)]"
	$(RM) $(NAME)

re: fclean all

run: re
	clear
	echo "[$(GREEN)running$(RESET)]"
	./$(NAME)

leaks: re
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

.SILENT:
