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
	if(philo->id % 2 == 1)
	{
		pthread_mutex_lock(&philo->write);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write);

		pthread_mutex_lock(&philo->next->fork);
		pthread_mutex_lock(&philo->write);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write);

		pthread_mutex_lock(&philo->fork);
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->write);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write);

		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo->write);
		printf("%lld %d has taken a fork\n", get_time(), philo->id);
		pthread_mutex_unlock(&philo->write);

		pthread_mutex_lock(&philo->next->fork);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_time_access);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->last_meal_time_access);

	pthread_mutex_lock(&philo->write);
	printf("%lld %d is eating\n", get_time(), philo->id);
	pthread_mutex_unlock(&philo->write);

	usleep(philo->shared->time_to_eat * 1000);

	pthread_mutex_lock(&philo->meals);
	philo->shared->meals_eaten++;
	pthread_mutex_unlock(&philo->meals);

}

void	sleep_and_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->write);
	printf("%lld %d is sleeping\n", get_time(), philo->id);
	pthread_mutex_unlock(&philo->write);

	pthread_mutex_unlock(&philo->next->fork);
	pthread_mutex_unlock(&philo->fork);

	usleep(philo->shared->time_to_sleep * 1000);

	pthread_mutex_lock(&philo->write);
	printf("%lld %d is thinking\n", get_time(), philo->id);
	pthread_mutex_unlock(&philo->write);
}
