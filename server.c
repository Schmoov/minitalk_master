#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char	*g_server_str;

void	write_bin(char *c)
{
	char	byte;

	for (int i = 0; i < 8; i++)
	{
		byte = (*c >> (7-i))&1;
		byte += '0';
		write(2, &byte, 1);
	}
	write(2, "\n", 1);
}
		
void	handler(int signal)
{
	static int	bit = 0;


	if (signal == SIGUSR2)
	{
		*g_server_str |= 1;
	}
	bit++;
	if (bit == 8)
	{
		g_server_str++;
		bit = 0;
	}
	else
		*g_server_str <<= 1;
}

int	main(void)
{
	struct sigaction	act;
	char	*og_ptr;

	g_server_str = malloc(100000);
	og_ptr = g_server_str;
	memset(g_server_str, 0, 100000);

	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);

	printf("%d\n", getpid());
	fflush(stdout);

		pause();
		write(1, og_ptr, g_server_str - og_ptr);
}
