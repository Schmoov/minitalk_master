

all:
	cc -g client.c -o client
	cc -g server.c -o server

fclean:
	rm -f client server

re: fclean all
