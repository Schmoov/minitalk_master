#include "minitalk.h"

volatile int g_client_ack;

void	client_pause(void)
{
	while (!g_client_ack);
	g_client_ack = 0;
}

void	client_handler(int signal, siginfo_t *info, void *cont)
{
	(void) signal;
	(void) info;
	(void) cont;
	g_client_ack = 1;
}

void	send_len(pid_t server_pid, int len)
{
	int	i;

	i = 8 * sizeof(int);
	while (i)
	{
		i--;
		if ((len >> i) & 1)
			kill(server_pid, SIGUSR2);
		else
			kill(server_pid, SIGUSR1);
		client_pause();
	}
}

void	send_string(pid_t server_pid, char *msg, int len)
{
	int	i;
	int	j;

	i = 0;
	while (i < len)
	{
		j = 8;
		while (j)
		{
			j--;
			if ((msg[i] >> j) & 1)
				kill(server_pid, SIGUSR2);
			else
				kill(server_pid, SIGUSR1);
			client_pause();
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	act;
	pid_t	server_pid;
	int		len;

	if (argc != 3)
		return (write(2, "Usage: ./client pid msg\n", 24));
	server_pid = ft_strtoll(argv[1], 0, 0);
	if (server_pid == -1)
		return (write(2, "I'd rather not...\n", 18));
	len = ft_strlen(argv[2]);
	ft_memset(&act, 0, sizeof(act));
	act.sa_sigaction = client_handler;
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	kill(server_pid, SIGUSR1);
	client_pause();
	send_len(server_pid, len);
	send_string(server_pid, argv[2], len);
	kill(server_pid, SIGUSR1);
	client_pause();
	write(1, "Great Success\n", 15);
}
