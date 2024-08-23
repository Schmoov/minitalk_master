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

	write(1, "Great Success\n", 15);
	//send_string();
}
