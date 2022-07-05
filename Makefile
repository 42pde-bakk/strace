NAME = ft_strace
INCLUDE = -Iinclude -Ilibft/include
HEADER = include/strace.h libft/include/libft.h

SRC_DIR = ./srcs
OBJ_DIR = ./obj
SRC_EXT = c
OBJ_EXT = o
#SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS = $(shell find $(SRC_DIR)/ -type f -name "*.${SRC_EXT}")
OBJECTSS = $(SRCS:.c=.o)
OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTSS))

LIBS = libft.a

# COLORS
PINK = \x1b[35;01m
BLUE = \x1b[34;01m
YELLOW = \x1b[33;01m
GREEN = \x1b[32;01m
RED = \x1b[31;01m
WHITE = \x1b[31;37m
RESET = \x1b[0m

CFLAGS = -Wall -Werror -Wextra -Wno-missing-braces
ifdef DEBUG
 CFLAGS += -g3 -fsanitize=address
endif
SHELL := /bin/bash
export SHELL
export DEBUG
ifdef SPEED
 CFLAGS += -Ofast
endif
ifdef MANDATORY
 CFLAGS += -D PRINT_STRINGS_AS_POINTERS=1
endif

all: $(NAME)

$(NAME): $(LIBS) $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBS) -o $@
	@printf "$(PINK)Done building $(NAME) $(RESET)\n"

%.a: %
	$(MAKE) -C $< && cp $</$@ .

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p "$(@D)"
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	/bin/rm -f $(OBJS)
	@/bin/rm -f *.o *~ *.gch
	$(MAKE) clean -C libft

fclean: clean
	/bin/rm -f $(NAME)
	$(MAKE) fclean -C libft

re: fclean all
