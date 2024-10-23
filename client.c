#include "minitalk.h"

static void	handler(int signal)
{
}

void	establish_connection(pid_t server_pid)
{
	kill(server_pid, SIGUSR1);
	pause();
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
		pause();
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
			pause();
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;
	int		len;

	if (argc != 3)
		return (write(2, "Usage: ./client pid msg\n", 24));
	server_pid = ft_strtoll(argv[1], 0, 0);
	if (server_pid == -1)
		return (write(2, "I'd rather not...\n", 18));
	len = ft_strlen(argv[2]);
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	establish_connection(server_pid);
	send_len(server_pid, len);
	send_string(server_pid, argv[2], len);
	write(1, "Great Success\n", 15);
}
