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
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		printf("%lldms %d has taken a fork\n", get_time(), philo->id);
		if(philo->sim_stop == 1)
		{
			pthread_mutex_unlock(&philo->fork);
			return ;
		}
		pthread_mutex_lock(&philo->next->fork);
		printf("%lldms %d has taken a fork\n", get_time(), philo->id);
		if(philo->sim_stop == 1)
		{
			pthread_mutex_unlock(&philo->next->fork);
			return ;
		}
	}
	if(philo->id % 2 == 1)
	{
		pthread_mutex_lock(&philo->next->fork);
		printf("%lldms %d has taken a fork\n", get_time(), philo->id);
		if(philo->sim_stop == 1)
		{
			pthread_mutex_unlock(&philo->next->fork);
			return ;
		}
		pthread_mutex_lock(&philo->fork);
		printf("%lldms %d has taken a fork\n", get_time(), philo->id);
		if(philo->sim_stop == 1)
		{
			pthread_mutex_unlock(&philo->fork);
			return ;
		}
	}
}

void	eat(t_philo *philo)
{
	philo->last_meal_time = get_time();
	printf("%lldms %d is eating\n", get_time(), philo->id);
	usleep(philo->shared->time_to_eat * 1000);

	pthread_mutex_lock(&philo->meals);
	philo->shared->meals_eaten++;
	pthread_mutex_unlock(&philo->meals);

	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
}

void	sleep_and_think(t_philo *philo)
{
	printf("%lldms %d is sleeping\n", get_time(), philo->id);
	usleep(philo->shared->time_to_sleep * 1000);

	printf("%lldms %d is thinking\n", get_time(), philo->id);
}
