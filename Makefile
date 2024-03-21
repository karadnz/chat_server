AR	= ar rcs
RM	= rm -f

CC				= gcc
CFLAGS_DEBUG	= -I ./include/ -fsanitize=address
CFLAGS			= -I ./include/ -pthread # -Wall -Wextra -Werror
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

# despite its name -g is default this target adds -fsanitize=adress
debug: $(OBJ) | $(BIN_DIR)
	@$(CC) $(CFLAGS_DEBUG) $(OBJ) $(ARC) $(FRMS) -o $(NAME)
	@echo "server data is compiled."

$(NAME): $(OBJ) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJ) $(ARC) $(FRMS) -o $(NAME)
	@echo "server is compiled."

$(CLIENT): $(OBJ) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJ) $(ARC) $(FRMS) -o $(NAME)
	@echo "server is compiled."

# find better way to debug
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC)  $(CFLAGS) $(CDEBUG) -c $< -o $@

client:
	@gcc ./client.c -o client
	@echo "client is compiled."

run: all
	./$(NAME) 20003

clean:
	rm -f $(OBJ)
	@echo "Cleaned"

fclean: clean
	rm -f $(NAME)
	rm -rf client
	@echo "Fully cleaned"
	

re: fclean
	@make all

.PHONY: all clean fclean run re client