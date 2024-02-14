/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:45:38 by aarbenin          #+#    #+#             */
/*   Updated: 2024/02/07 12:45:42 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minitalk.h"

volatile int	g_confirmation_received = 0;

void	confirmation_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGUSR1 || sig == SIGUSR2)
	{
		g_confirmation_received = 1;
		write(1, "confirmation received\n", 23);
	}
}

void	handle_error(int signal)
{
	if (signal == SIGUSR1)
		write(STDERR_FILENO, "Error sending SIGUSR1\n", 23);
	else if (signal == SIGUSR2)
		write(STDERR_FILENO, "Error sending SIGUSR2\n", 23);
	exit(1);
}

void	send_char(char c, int pid)
{
	static int	i;
	int			bit;

	i = 7;
	while (i >= 0)
	{
		bit = ((c) >> i) & 1;
		if (bit == 0)
		{
			if (kill(pid, SIGUSR1) == -1)
				handle_error(SIGUSR1);
		}
		else
		{
			if (kill(pid, SIGUSR2) == -1)
				handle_error(SIGUSR2);
		}
		i--;
		while (!g_confirmation_received)
			pause();
		g_confirmation_received = 0;
		usleep(1000);
	}
}

void	validate_args(int argc, char **argv)
{
	int	i;

	if (argc != 3)
	{
		write(STDERR_FILENO, "Usage: <pid> <message>\n", 24);
		exit(1);
	}
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
		{
			write(STDERR_FILENO, "Invalid PID, use only digits\n", 30);
			exit(1);
		}
		i++;
	}
	if (ft_atoi(argv[1]) <= 0)
	{
		write (STDERR_FILENO, "Invalid PID, use number bigger then 0\n", 39);
		exit(1);
	}
}

int	main(int argc, char **argv)
{
	int					pid;
	int					i;
	char				*str;
	struct sigaction	sa;

	validate_args(argc, argv);
	i = 0;
	pid = atoi(argv[1]);
	str = argv[2];
	sa.sa_sigaction = &confirmation_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (str[i])
	{
		send_char(str[i], pid);
		i++;
	}
	return (0);
}
