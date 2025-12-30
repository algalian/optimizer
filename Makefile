NAME        = optimizer
WINNAME     = optimizer.exe

CC          = gcc
WINCC       = x86_64-w64-mingw32-gcc

CFLAGS      = -Wall -D BUFFER_SIZE=64 -I. -Ilibft
LIBS        = -lm

# Project sources
SRC         = main.c \
              read_csv.c \
			  read_csv_utils.c \
              get_next_line.c \
              logic_engine.c \
			  sort.c \
			  lists.c \
			  write_csv.c \
			  parser_csv.c \
			  parser_interface.c \
			  col_mapping.c \
			  loader.c \
			  parser_tsv.c \

OBJ         = $(SRC:.c=.o)

# Libft sources
LIBFT_DIR   = libft
LIBFT_SRC   = $(wildcard $(LIBFT_DIR)/*.c)
LIBFT_OBJ   = $(LIBFT_SRC:.c=.o)

GREEN       = \033[0;32m
YELLOW      = \033[0;33m
RESET       = \033[0m


# ---------- BUILD RULES ----------

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_OBJ) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Build complete: $(NAME)$(RESET)"


# Compile project sources
%.o: %.c optimizer.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiled: $<$(RESET)"

# Compile libft files
libft/%.o: libft/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiled libft: $<$(RESET)"


# ---------- WINDOWS BUILD ----------

win:
	@$(WINCC) $(CFLAGS) $(SRC) $(LIBFT_SRC) $(LIBS) -o $(WINNAME)
	@echo "$(GREEN)Windows executable built: $(WINNAME)$(RESET)"


# ---------- CLEAN RULES ----------

clean:
	@rm -f $(OBJ) $(LIBFT_OBJ)
	@echo "$(YELLOW)Object files removed.$(RESET)"

fclean: clean
	@rm -f $(NAME) $(WINNAME)
	@echo "$(YELLOW)Executables removed.$(RESET)"

re: fclean all


.PHONY: all clean fclean re win
