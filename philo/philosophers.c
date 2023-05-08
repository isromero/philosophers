/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:32:48 by isromero          #+#    #+#             */
/*   Updated: 2023/04/27 09:32:48 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*check_philosophers(void *args)
{
	t_philo 	*philo;
	int i;

	philo = (t_philo *)args;
	i = 0;

	while(1)
	{
        if (get_time() - philo->last_meal_time >= philo->time_to_die)
        {
            pthread_mutex_lock(&philo->args.lock_print);
            printf("%lld %d died\n", get_time(), philo->id);
			pthread_mutex_unlock(&philo->args.lock_print);
			while (i < philo[i].n_philos)
			{
				philo[i].stop = 1;
				i++;
			}
			pthread_join(philo->check, NULL);
			return (0);
        }
    }
	return (0);
}

void	*routine(void *args)
{
	t_philo 	*philo;
	philo = (t_philo *)args;

	pthread_create(&philo->check, NULL, check_philosophers, philo);
	while(philo->stop == 0)
	{
		if (philo->n_philos == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(&philo->args.lock_print);
			printf("%lld %d has taken a fork\n", get_time(), philo->id);
			pthread_mutex_unlock(&philo->args.lock_print);
			pthread_join(philo->check, NULL);
			return(0);
		}
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	pthread_join(philo->check, NULL);
	return(0);
}

int	main(int argc, char **argv)
{
	t_args 		args;

	if(!parse_args(argc, argv, &args))
		return (1);

	init_forks(&args);
	init_philos(&args);

	return (0);
}