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

void	handle_signal(int sig, siginfo_t *info, void *context)
{
	static unsigned char	current_byte;
	static int				counter;
	int						bit;

	(void)context;
	if (sig == SIGUSR1)
		bit = 0;
	else
		bit = 1;
	current_byte = (current_byte << 1) | bit;
	counter++;
	if (counter == 8)
	{
		write(1, &current_byte, 1);
		current_byte = 0;
		counter = 0;
	}
	if (sig == SIGUSR1)
		kill(info->si_pid, SIGUSR1);
	else
		kill(info->si_pid, SIGUSR2);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_printf("My PID: %d\n", getpid());
	ft_printf("Waiting for signal...\n");
	while (1)
	{
		pause();
	}
	return (0);
}
