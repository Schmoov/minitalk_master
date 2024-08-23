#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	bool	connected;
	int		bit_len;
	size_t	len;
	int		bit_msg;
	char	*msg;
}			t_minitalk_s;

t_minitalk_s	g_serv;

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
		
void	handler(int signal, siginfo_t *info, void *cont)
{
	if (!g_serv.connected)
		g_serv.connected = true;
	else if (g_serv.bit_len < 8 * sizeof(size_t))
	{
		g_serv.len <<= 1;
		if (info->si_signo == SIGUSR2)
			g_serv.len |= 1;
		g_serv.bit_len++;
	}
		
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	memset(&g_serv, 0, sizeof(g_serv));
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);

	printf("%d\n", getpid());
	fflush(stdout);

	while (1)
	{
		if (g_serv.bit_len == 8 * sizeof(size_t))
		{
			printf("%zu\n", g_serv.len);
			memset(&g_serv, 0, sizeof(g_serv));
		}
		pause();
	}
}
