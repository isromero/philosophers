/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:50:34 by isromero          #+#    #+#             */
/*   Updated: 2023/05/02 20:11:32 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_time(void)
{
	static struct timeval		unix_clock;
	struct timeval				current_time;

	if (unix_clock.tv_sec == 0 && unix_clock.tv_usec == 0)
	{
		if (gettimeofday(&unix_clock, NULL) == -1)
		{
			printf("Error getting time of day\n");
			return (0);
		}
	}
	if (gettimeofday(&current_time, NULL) == -1)
	{
		printf("Error getting time of day\n");
		return (0);
	}
	return (((current_time.tv_sec - unix_clock.tv_sec) * 1000) + \
	((current_time.tv_usec - unix_clock.tv_usec) / 1000));
}

int	ft_atoi(const char *str)
{
	int					p;
	int					n;
	unsigned int		res;

	p = 0;
	n = 1;
	res = 0;
	while ((str[p] >= 9 && str[p] <= 13) || (str[p] == 32))
		p++;
	if (str[p] == '+' || str[p] == '-')
	{
		if (str[p] == '-')
			n = n * -1;
		p++;
	}
	while (str[p] >= '0' && str[p] <= '9')
	{
		res = (str[p] - '0') + (res * 10);
		p++;
	}
	if (res > 2147483648 && n == -1)
		return (0);
	if (res > 2147483647 && n == 1)
		return (-1);
	return (res * n);
}

void	log_message(t_philo *philo, t_state state)
{
	const char	*states[6] = {DEAD_STR, EAT_STR, THINK_STR,
		SLEEP_STR, TAKE_FORK_STR};

	if (philo->args->stop_sim)
		return ;
	pthread_mutex_lock(&philo->args->lock_print);
	printf("%lld %d %s", get_time(), philo->id, states[state]);
	pthread_mutex_unlock(&philo->args->lock_print);
}
