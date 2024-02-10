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

/* The check_death function continuously checks if a philosopher has exceeded the time limit for not eating. 
If the philosopher has not eaten for too long, the function logs their death, stops the simulation, and exits 
the thread. */
void	*check_death(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(&philo->args->lock_stop_sim);
		if (philo->args->stop_sim)
		{
			pthread_mutex_unlock(&philo->args->lock_stop_sim);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->args->lock_stop_sim);
		if ((get_time() - philo->last_meal_time) > philo->time_to_die)
		{
			pthread_mutex_lock(&philo->args->lock_death);
			log_message(philo, DEAD);
			pthread_mutex_lock(&philo->args->lock_stop_sim);
			philo->args->stop_sim = true;
			pthread_mutex_unlock(&philo->args->lock_stop_sim);
			pthread_mutex_unlock(&philo->args->lock_death);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

/* This function checks if all the meals have been eaten by the philosophers. It does this by continuously checking 
if the number of meals eaten by all the philosophers is greater than or equal to the total number of meals that were 
supposed to be eaten. If this condition is met, the function stops the simulation and returns NULL. It also uses mutex 
locks to ensure thread safety while accessing shared variables. */
void	*check_meals(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(&philo->args->lock_stop_sim);
		if (philo->args->stop_sim)
		{
			pthread_mutex_unlock(&philo->args->lock_stop_sim);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->args->lock_stop_sim);
		pthread_mutex_lock(&philo->args->lock_meals_eaten);
		if (philo->n_meals > 0 && (philo->args->meals_eaten >= \
		(philo->n_meals * philo->n_philos)))
		{
			pthread_mutex_unlock(&philo->args->lock_meals_eaten);
			pthread_mutex_lock(&philo->args->lock_meals_stop);
			pthread_mutex_lock(&philo->args->lock_stop_sim);
			philo->args->stop_sim = true;
			pthread_mutex_unlock(&philo->args->lock_stop_sim);
			pthread_mutex_unlock(&philo->args->lock_meals_stop);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->args->lock_meals_eaten);
		usleep(1000);
	}
	return (NULL);
}
