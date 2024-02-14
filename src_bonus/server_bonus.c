/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:46:05 by aarbenin          #+#    #+#             */
/*   Updated: 2024/02/07 12:46:07 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minitalk.h"

static volatile sig_atomic_t	g_sig_state[2] = {};

void	collect_str(unsigned char c)
{
	static unsigned char	str[1024];
	static int				i;

	if (c != '\0')
	{
		str[i++] = c;
		if (i == 1024)
		{
			write(1, str, i);
			i = 0;
		}
	}
	else
	{
		if (i > 0)
		{
			write(1, str, i);
			i = 0;
		}
		write(1, "\n", 1);
	}
}

void	handle_signal(int sig, siginfo_t *info, void *context)
{
	static unsigned char	current_byte;
	static int				bit_counter;

	(void)context;
	if (sig == SIGUSR2)
		current_byte |= (1 << bit_counter);
	bit_counter++;
	if (bit_counter == 8)
	{
		collect_str(current_byte);
		current_byte = 0;
		bit_counter = 0;
	}
	g_sig_state[0] = 1;
	g_sig_state[1] = info->si_pid;
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &handle_signal;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || \
	sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_printf("Error handling signal/n");
		exit(EXIT_FAILURE);
	}
	ft_printf("My PID: %d\n", getpid());
	ft_printf("Waiting for signal...\n");
	while (1)
	{
		while (!g_sig_state[0])
			;
		g_sig_state[0] = 0;
		kill(g_sig_state[1], SIGUSR1);
	}
	return (0);
}
