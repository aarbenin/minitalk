/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 13:52:47 by aarbenin          #+#    #+#             */
/*   Updated: 2024/02/03 13:52:51 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minitalk.h"

void	handle_error(int signal)
{
	if (signal == SIGUSR1)
		write(STDERR_FILENO, "Error sending SIGUSR1\n", 23);
	else if (signal == SIGUSR2)
		write(STDERR_FILENO, "Error sending SIGUSR2\n", 23);
	exit(EXIT_FAILURE);
}

void	send_char(int pid, int character)
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
		usleep(100);
	}
}

void	validate_args(int argc, char **argv)
{
	int	i;

	if (argc != 3)
	{
		write(STDERR_FILENO, "Usage: <pid> <message>\n", 24);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
		{
			write(STDERR_FILENO, "Invalid PID, use only digits\n", 30);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	if (ft_atoi(argv[1]) <= 0 || kill(ft_atoi(argv[1]), 0) == -1)
	{
		write (STDERR_FILENO, "PID error\n", 39);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv)
{
	int		pid;
	int		i;
	char	*str;

	validate_args(argc, argv);
	pid = ft_atoi(argv[1]);
	str = argv[2];
	i = 0;
	while (str[i])
		send_char(pid, str[i++]);
	send_char(pid, 0);
	return (0);
}
