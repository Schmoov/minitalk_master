CC = cc -Wall -Wextra -Werror -g
LIBFT:=libft/libft.a

$(LIBFT):
	make -C libft
all:
	$(CC) client.c $(LIBFT) -o client
	$(CC) server.c $(LIBFT) -o server

fclean:
	rm -f client server

re: fclean all
