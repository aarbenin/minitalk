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

static t_server_state	g_state = {0};

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
	g_state.signal_received = 1;
	g_state.client_pid = info->si_pid;
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
		ft_printf("Error handling signal\n");
		exit(EXIT_FAILURE);
	}
	ft_printf("My PID: %d\n", getpid());
	ft_printf("Waiting for signal...\n");
	while (1)
	{
		while (!g_state.signal_received)
			usleep(150);
		g_state.signal_received = 0;
		kill(g_state.client_pid, SIGUSR1);
	}
	return (0);
}
