#include "minitalk.h"

volatile int g_serv_ack;

void	serv_pause(void)
{
	while (!g_serv_ack);
}

static void	serv_handler(int signal, siginfo_t *info, void *cont)
{
	int global;

	(void) cont;
	if (signal == SIGUSR2)
		global = info->si_pid, write(1,"1",0);
	else
		global = -info->si_pid, write(1,"0",0);
	g_serv_ack = global;
}

void	receive_len(t_minitalk *mntk)
{
	int		i;
	pid_t	client_pid;

	i = 0;
	while (i < 8 * (int)sizeof(int))
	{
		serv_pause();
		mntk->len <<= 1;
		if (g_serv_ack > 0)
			mntk->len |= 1;
		else
			g_serv_ack = -g_serv_ack;
		i++;
		client_pid = g_serv_ack;
		g_serv_ack = 0;
		if (i == 8 * (int)sizeof(int))
			mntk->msg = ft_calloc(mntk->len, 1);
		kill(client_pid, SIGUSR1);
	}
}

void	receive_body(t_minitalk *mntk)
{
	int		i;
	pid_t	client_pid;

	i = 0;
	while (i < 8 * mntk->len)
	{
		serv_pause();
		mntk->msg[i / 8] <<= 1;
		if (g_serv_ack > 0)
			mntk->msg[i / 8] |= 1;
		else
			g_serv_ack = -g_serv_ack;
		i++;
		ft_printf("%d ", i);
		client_pid = g_serv_ack;
		g_serv_ack = 0;
		if (i == 8 * mntk->len)
		{
			write(1, mntk->msg, mntk->len);
			free(mntk->msg);
		}
		kill(client_pid, SIGUSR1);
	}
}


int	main(void)
{
	struct sigaction	act;
	t_minitalk			mntk;

	ft_memset(&act, 0, sizeof(act));
	ft_memset(&mntk, 0, sizeof(mntk));
	act.sa_sigaction = serv_handler;
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	ft_printf("%d\n", getpid());
	while (1)
	{
		receive_len(&mntk);
		receive_body(&mntk);
	}
}
