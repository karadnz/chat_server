AR	= ar rcs
RM	= rm -f

CC				= gcc
CFLAGS_DEBUG	= -I ./include/ -fsanitize=address
CFLAGS			= -I ./include/ # -Wall -Wextra -Werror
CDEBUG			= -g 




SRC_DIR	= src
OBJ_DIR	= obj
BIN_DIR	= bin

SRC =	server.c server_utils.c commands.c message.c\
		client_utils.c queue.c \
		utils.c \
		
		

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# ARC = ./src/gnl/get_next_line.a mlx/libmlx.a


BIN		= server
NAME	= $(BIN_DIR)/$(BIN)


all: $(NAME)

# -g is default this target adds -fsanitize=adress
debug: $(OBJ) | $(BIN_DIR)
	@$(CC) $(CFLAGS_DEBUG) $(OBJ) $(ARC) $(FRMS) -o $(NAME)
	@echo "cub3d data is compiled."

$(NAME): $(OBJ) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJ) $(ARC) $(FRMS) -o $(NAME)
	@echo "cub3d is compiled."

# find better way to debug
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC)  $(CFLAGS) $(CDEBUG) -c $< -o $@

run: all
	./$(NAME) 20003

clean:
	rm -f $(OBJ)
	@echo "Cleaned"

fclean: clean
	rm -f $(NAME)
	@echo "Fully cleaned"
	

re: fclean
	@make all

.PHONY: all clean fclean run re