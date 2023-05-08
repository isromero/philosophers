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

#include "philosophers.h"

void	*check_death(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!philo->args->stop_sim)
	{
        if ((get_time() - philo->last_meal_time) >= philo->args->time_to_die)
        {
			pthread_join(philo->death_check, NULL);
			pthread_mutex_lock(&philo->args->lock_death);
			log_message(philo, DEAD);
			philo->args->stop_sim = true;
			pthread_mutex_unlock(&philo->args->lock_death);
			return (NULL);
        }
    }
	return (NULL);
}

void	*check_meals(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!philo->args->stop_sim)
	{
		if(philo->args->n_meals > 0 && (philo->args->meals_eaten >= (philo->args->n_meals * philo->args->n_philos)))
		{
			pthread_join(philo->meals_check, NULL);
			pthread_mutex_lock(&philo->args->lock_meals_stop);
			philo->args->stop_sim = true;
			pthread_mutex_unlock(&philo->args->lock_meals_stop);
			return (NULL);
		}	
    }
	return (NULL);
}

void	*routine(void *args)
{
	t_philo 	*philo;

	philo = (t_philo *)args;
	pthread_create(&philo->death_check, NULL, check_death, philo);
	pthread_create(&philo->meals_check, NULL, check_meals, philo);
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