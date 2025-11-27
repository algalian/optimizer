# **************************************************************************** #
#                                   MAKEFILE                                   #
# **************************************************************************** #

NAME		= optimizer
WINNAME		= optimizer.exe
CC			= gcc
WINCC		= x86_64-w64-mingw32-gcc
CFLAGS		= -Wall -D BUFFER_SIZE=64
LIBS		= -lm

SRC			= main.c \
			  read_csv.c \
			  get_next_line.c \
			  get_next_line_utils.c \
			  logic_engine.c \
			  
OBJ			= $(SRC:.c=.o)

GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Build complete: $(NAME)$(RESET)"


win:
	@$(WINCC) $(CFLAGS) $(SRC) $(LIBS) -o $(WINNAME)
	@echo "$(GREEN)Windows executable built: $(WINNAME)$(RESET)"

%.o: %.c optimizer.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiled: $<$(RESET)"

clean:
	@rm -f $(OBJ)
	@echo "$(YELLOW)Object files removed.$(RESET)"

fclean: clean
	@rm -f $(NAME) $(WINNAME)
	@echo "$(YELLOW)Executables removed.$(RESET)"
re: fclean all

.PHONY: all clean fclean re