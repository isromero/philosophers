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

/* This function returns the elapsed time in milliseconds since an arbitrary fixed point in the past. 
It uses the gettimeofday function to get the current time and subtracts it from the time the function 
was first called to get the elapsed time. If the gettimeofday function fails, it returns 0 and prints 
an error message. */
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

/* This is a libft function. The function converts a string to an integer by skipping whitespace and checking 
for a sign. It then reads through the string, converting each digit to an integer and adding it to a running 
total. If the resulting integer is too large, it returns 0 or -1, otherwise it returns the integer value. */
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

/* This function logs a message with the current time, philosopher id, and state of the philosopher's action. 
It uses a mutex lock to ensure thread-safety when accessing the console. If the simulation has been stopped, 
the function returns without printing anything(you need that if() 100%) */
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
