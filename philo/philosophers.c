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
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!philo->args->stop_sim)
	{
        if ((get_time() - philo->last_meal_time) >= philo->args->time_to_die)
        {
			log_message(philo, DEAD);
			philo->args->stop_sim = true;
			return (NULL);
        }
    }
	return (NULL);
}

void	*routine(void *args)
{
	t_philo 	*philo;

	philo = (t_philo *)args;
	pthread_create(&philo->check, NULL, check_philosophers, philo);
	while (!philo->args->stop_sim)
	{
		if (philo->args->n_philos == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			log_message(philo, FORK);
			return (NULL);
		}
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_args 		args;

	if (parse_args(argc, argv, &args) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	init_forks(&args);
	init_philos(&args);
	return (EXIT_SUCCESS);
}
