LIBFT:=libft/libft.a

$(LIBFT):
	make -C libft
all:
	cc -g client.c $(LIBFT) -o client
	cc -g server.c $(LIBFT) -o server

fclean:
	rm -f client server

re: fclean all
