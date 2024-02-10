NAME = client server
BONUS_NAME = client_bonus server_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_CLIENT = src/client.c
SRC_SERVER = src/server.c
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
OBJ_SERVER = $(SRC_SERVER:.c=.o)

SRC_BONUS_CLIENT = src_bonus/client_bonus.c
SRC_BONUS_SERVER = src_bonus/server_bonus.c
OBJ_BONUS_CLIENT = $(SRC_BONUS_CLIENT:.c=.o)
OBJ_BONUS_SERVER = $(SRC_BONUS_SERVER:.c=.o)

LIBFT = libft
LIBFT_LIB = $(LIBFT)/libft.a

all: $(NAME)

$(NAME): $(OBJ_CLIENT) $(OBJ_SERVER) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -o client $(OBJ_CLIENT) -L$(LIBFT) -lft
	$(CC) $(CFLAGS) -o server $(OBJ_SERVER) -L$(LIBFT) -lft

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(OBJ_BONUS_CLIENT) $(OBJ_BONUS_SERVER) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -o client_bonus $(OBJ_BONUS_CLIENT) -L$(LIBFT) -lft
	$(CC) $(CFLAGS) -o server_bonus $(OBJ_BONUS_SERVER) -L$(LIBFT) -lft

clean:
	$(MAKE) clean -C $(LIBFT)
	rm -f $(OBJ_CLIENT) $(OBJ_SERVER) $(OBJ_BONUS_CLIENT) $(OBJ_BONUS_SERVER)

fclean: clean
	$(MAKE) fclean -C $(LIBFT)
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
