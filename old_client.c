#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>


static void	handler(int signal)
{
}

void	establish_connection(pid_t server_pid)
{
	kill(server_pid, SIGUSR1);
	pause();
}

void	send_len(pid_t server_pid, size_t len)
{
	int	i;

	i = 8 * sizeof(size_t);
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

void	send_string(pid_t server_pid, char *msg)
{
	int	i;
	int	j;

	i = 0;
	while (i < strlen(msg))
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
	struct sigaction	act;
	pid_t				server_pid = atoi(argv[1]);
	size_t				len = strlen(argv[2]);

	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	establish_connection(server_pid);
	send_len(server_pid, len);
	send_string(server_pid, argv[2]);

	write(1, "Great Success\n", 15);
}
