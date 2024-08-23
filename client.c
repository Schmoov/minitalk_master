#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	for (int i = 0; i < strlen(argv[2]); i++)
		for (int j = 0; j < 8; j++)
		{
			if (argv[2][i] & (1 << (7 - j)))
				kill(atoi(argv[1]), SIGUSR2);
			else
				kill(atoi(argv[1]), SIGUSR1);
			usleep(10);
		}
}
