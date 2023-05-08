/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:59:16 by isromero          #+#    #+#             */
/*   Updated: 2023/05/08 20:59:16 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		usleep(50);
	}
	return (NULL);
}

void	*check_meals(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!philo->args->stop_sim)
	{
		if (philo->args->n_meals > 0 && (philo->args->meals_eaten >= \
		(philo->args->n_meals * philo->args->n_philos)))
		{
			pthread_join(philo->meals_check, NULL);
			pthread_mutex_lock(&philo->args->lock_meals_stop);
			philo->args->stop_sim = true;
			pthread_mutex_unlock(&philo->args->lock_meals_stop);
			return (NULL);
		}
		usleep(50);
	}
	return (NULL);
}
