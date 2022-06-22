NAME = ft_strace
INCLUDE = -Iinclude
HEADER = include/strace.h

SRC_DIR = ./srcs
OBJ_DIR = ./obj
SRC_EXT = c
OBJ_EXT = o
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJECTSS = $(SRCS:.c=.o)
OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTSS))

# COLORS
PINK = \x1b[35;01m
BLUE = \x1b[34;01m
YELLOW = \x1b[33;01m
GREEN = \x1b[32;01m
RED = \x1b[31;01m
WHITE = \x1b[31;37m
RESET = \x1b[0m

CFLAGS = -Wall -Werror -Wextra -std=c11 -Wno-missing-braces
ifdef DEBUG
 CFLAGS += -g3 -fsanitize=address
endif
SHELL := /bin/bash
export SHELL
export DEBUG

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@
	@printf "$(PINK)Done building $(NAME) $(RESET)\n"

directories:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MAKE) -s directories
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	/bin/rm -f $(OBJS)
	@/bin/rm -f *.o *~ *.gch

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
