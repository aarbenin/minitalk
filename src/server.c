/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 13:52:28 by aarbenin          #+#    #+#             */
/*   Updated: 2024/02/03 13:52:31 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minitalk.h"

void	collect_str(unsigned char c)
{
	static unsigned char	str[1024];
	static int				i;

	if (c != '\0')
	{
		str[i++] = c;
		if (i == 1023)
		{
			str[i] = '\0';
			write(1, str, i);
			i = 0;
		}
	}
	else
	{
		if (i > 0)
		{
			str[i] = '\0';
			write(1, str, i);
			i = 0;
		}
		write(1, "\n", 1);
	}
}

void	handle_signal(int sig)
{
	static unsigned char	current_byte;
	static int				bit_counter;

	if (sig == SIGUSR2)
		current_byte |= (1 << bit_counter);
	bit_counter++;
	if (bit_counter == 8)
	{
		collect_str(current_byte);
		current_byte = 0;
		bit_counter = 0;
	}
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || \
	sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(STDERR_FILENO, "Error handling signal\n", 23);
		exit(EXIT_FAILURE);
	}
	ft_printf("My PID: %d\n", getpid());
	ft_printf("Waiting for signal...\n");
	while (1)
	{
		pause();
	}
	return (0);
}
