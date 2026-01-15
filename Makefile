NAME        = optimizer
WINNAME     = optimizer.exe
WINFOLDER   = optimizer_windows

CC          = gcc
WINCC       = x86_64-w64-mingw32-gcc

CFLAGS      = -Wall -D BUFFER_SIZE=64 -I. -Ilibft
WINFLAGS    = -Wall -D BUFFER_SIZE=64 -I. -Ilibft -I/mingw64/include

LIBS = -lm -lxlsxio_read -lz -lminizip -lexpat
WINLIBS     = -L/mingw64/lib -lxlsxio_read -lz -lminizip -lexpat

SRC = main.c logic_engine.c sort.c lists.c loader.c \
      parser_csv.c parser_tsv.c parser_xlsx.c parser_interface.c \
      record_reader.c tokenizer_csv.c tokenizer_tsv.c \
      col_mapping.c col_schema.c numeric.c write.c \

OBJ = $(SRC:.c=.o)

LIBFT_DIR = libft
LIBFT_SRC = $(wildcard $(LIBFT_DIR)/*.c)
LIBFT_OBJ = $(LIBFT_SRC:.c=.o)

GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

# ---------- LINUX BUILD ----------

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_OBJ) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Linux build complete: $(NAME)$(RESET)"

%.o: %.c optimizer.h
	@echo "$(YELLOW)$<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

libft/%.o: libft/%.c
	@echo "$(YELLOW)$<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# ---------- WINDOWS BUILD ----------

win:
	@mkdir -p $(WINFOLDER)
	@$(WINCC) $(WINFLAGS) $(SRC) $(LIBFT_SRC) $(WINLIBS) -o $(WINFOLDER)/$(WINNAME)
	@echo "$(GREEN)Windows build complete: $(WINFOLDER)/$(WINNAME)$(RESET)"
	@mingw32-make win-dlls

win-dlls:
	@cp /mingw64/bin/libxlsxio_read.dll $(WINFOLDER)/
	@cp /mingw64/bin/libminizip-*.dll $(WINFOLDER)/
	@cp /mingw64/bin/zlib1.dll $(WINFOLDER)/
	@cp /mingw64/bin/libexpat-1.dll $(WINFOLDER)/
	@cp /mingw64/bin/libbz2-1.dll $(WINFOLDER)/
	@cp /mingw64/bin/libgcc_s_seh-1.dll $(WINFOLDER)/
	@cp /mingw64/bin/libstdc++-6.dll $(WINFOLDER)/
	@cp /mingw64/bin/libwinpthread-1.dll $(WINFOLDER)/
	@echo "$(GREEN)All DLLs copied to $(WINFOLDER)$(RESET)"

# ---------- CLEAN ----------

clean:
	@rm -f $(OBJ) $(LIBFT_OBJ)
	@rm -f *.dll
	@rm -f $(WINNAME)
	@rm -rf $(WINFOLDER)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all win clean fclean re
