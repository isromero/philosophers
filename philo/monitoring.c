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

static void	check_death_stop(t_philo *philo)
{
	pthread_mutex_unlock(&philo->args->lock_last_meal_time);
	pthread_mutex_lock(&philo->args->lock_death);
	log_message(philo, DEAD);
	pthread_mutex_lock(&philo->args->lock_stop_sim);
	philo->args->stop_sim = true;
	pthread_mutex_unlock(&philo->args->lock_death);
	pthread_mutex_unlock(&philo->args->lock_stop_sim);
}

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
		pthread_mutex_lock(&philo->args->lock_last_meal_time);
		if ((get_time() - philo->last_meal_time) > philo->time_to_die)
		{
			check_death_stop(philo);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->args->lock_last_meal_time);
		usleep(1000);
	}
	return (NULL);
}

static void	check_meals_stop(t_philo *philo)
{
	pthread_mutex_unlock(&philo->args->lock_meals_eaten);
	pthread_mutex_lock(&philo->args->lock_meals_stop);
	pthread_mutex_lock(&philo->args->lock_stop_sim);
	philo->args->stop_sim = true;
	pthread_mutex_unlock(&philo->args->lock_meals_stop);
	pthread_mutex_unlock(&philo->args->lock_stop_sim);
}

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
			check_meals_stop(philo);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->args->lock_meals_eaten);
		usleep(1000);
	}
	return (NULL);
}
