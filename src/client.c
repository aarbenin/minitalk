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
		usleep(500);
	}
}

int	main(int argc, char **argv)
{
	int		pid;
	int		i;
	char	*str;

	if (argc != 3)
	{
		write(STDERR_FILENO, "Usage: <pid> <message>\n", 24);
		return (1);
	}
	i = 0;
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
		write(STDERR_FILENO, "Invalid PID\n", 12);
		return (1);
	}
	str = argv[2];
	while (str[i])
	{
		send_char(str[i], pid);
		i++;
	}
	return (0);
}
