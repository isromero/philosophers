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

/* we implement an asymmetric solution, and odd philosophers picks up first his left
fork and then his right fork, and an even philosophers picks up first right fork and then
his left fork, so they can't get in trouble with the order and we protect a data race */

void take_forks(t_philo *philo)
{
	/* pintf("[%d] is trying to take forks\n", philo->id); */
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write_mutex);

		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write_mutex);
	}
	if(philo->id % 2 == 0)
	{
		usleep(10);

		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write_mutex);

		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write_mutex);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->write_mutex);
	printf("%lld %d is eating\n", get_time(), philo->id);
	pthread_mutex_unlock(&philo->write_mutex);

	usleep(philo->data->time_eat * 1000);

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);

	pthread_mutex_lock(&philo->meals_eaten_mutex);
	philo->data->meals_eaten++;
	pthread_mutex_unlock(&philo->meals_eaten_mutex);

	pthread_mutex_lock(&philo->meal_time);
	philo->data->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->meal_time);
}

void	sleep_and_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->write_mutex);
	printf("%lld %d is sleeping\n", get_time(), philo->id);
	pthread_mutex_unlock(&philo->write_mutex);
	
	usleep(philo->data->time_sleep * 1000);

	pthread_mutex_lock(&philo->write_mutex);
	printf("%lld %d is thinking\n", get_time(), philo->id);
	pthread_mutex_unlock(&philo->write_mutex);
}