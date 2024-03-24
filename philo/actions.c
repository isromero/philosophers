/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 11:25:51 by isromero          #+#    #+#             */
/*   Updated: 2023/04/27 11:25:51 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		log_message(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		log_message(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		log_message(philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		log_message(philo, FORK);
	}
}

void	eat(t_philo *philo)
{
	long long int	end_time;

	pthread_mutex_lock(&philo->args->lock_last_meal_time);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->args->lock_last_meal_time);
	log_message(philo, EAT);
	end_time = get_time() + philo->time_to_eat;
	while (get_time() < end_time)
		usleep(200);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->args->lock_meals_eaten);
	philo->args->meals_eaten++;
	pthread_mutex_unlock(&philo->args->lock_meals_eaten);
}

void	sleep_and_think(t_philo *philo)
{
	long long int	end_time;

	end_time = get_time() + philo->time_to_sleep;
	log_message(philo, SLEEP);
	while (get_time() < end_time)
		usleep(200);
	log_message(philo, THINK);
}
