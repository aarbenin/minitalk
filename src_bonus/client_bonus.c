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
#include <errno.h>
#include "../include/minitalk.h"

volatile sig_atomic_t	g_confirmation_received = 0;

static void	confirmation_handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (sig == SIGUSR1)
		g_confirmation_received = 1;
}

void	handle_error(int signal)
{
	if (signal == SIGUSR1)
		write(STDERR_FILENO, "Error sending SIGUSR1\n", 23);
	else if (signal == SIGUSR2)
		write(STDERR_FILENO, "Error sending SIGUSR2\n", 23);
	exit(EXIT_FAILURE);
}

static void	send_char(int pid, int character, size_t *str_len)
{
	int	counter;
	int	bit;

	counter = 0;
	while (counter <= 7)
	{
		bit = (character >> counter) & 1;
		if (bit)
		{
			if (kill(pid, SIGUSR2) == -1)
				handle_error(SIGUSR2);
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
				handle_error(SIGUSR1);
		}
		counter++;
		while (g_confirmation_received != 1)
			usleep(150);
		g_confirmation_received = 0;
	}
	if (character != 0)
		(*str_len)++;
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
	if (ft_atoi(argv[1]) <= 0 || kill(ft_atoi(argv[1]), 0) == -1)
	{
		write (STDERR_FILENO, "PID error\n", 39);
		exit(1);
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	int					i;
	size_t				str_len;
	char				*str;
	int					pid;

	validate_args(argc, argv);
	str = argv[2];
	pid = ft_atoi(argv[1]);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = confirmation_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	i = 0;
	str_len = 0;
	while (str[i])
		send_char(pid, str[i++], &str_len);
	send_char(pid, 0, &str_len);
	ft_printf("Server received %u bytes (from PID %u)\n", str_len, getpid());
	return (0);
}
