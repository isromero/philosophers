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
	philo->state = 1;
	/* pintf("[%d] is trying to take forks\n", philo->id); */
	if (philo->id % 2 == 0)
	{
		if(philo->data->forks_available[philo->id] == 1)
			return ;
		pthread_mutex_lock(philo->left_fork);
		philo->data->forks_available[philo->id - 1] = 1;
		philo->data->current_time = get_time();
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", philo->data->current_time, philo->id);
		pthread_mutex_unlock(&philo->write_mutex);
		pthread_mutex_lock(philo->right_fork);
		philo->data->forks_available[philo->id] = 1;
		philo->data->current_time = get_time();
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", philo->data->current_time, philo->id);
		pthread_mutex_unlock(&philo->write_mutex);
	}
	if(philo->id % 2 == 1)
	{
		if(philo->data->forks_available[philo->id - 1] == 1)
			return ;
		pthread_mutex_lock(philo->right_fork);
		philo->data->forks_available[philo->id] = 1;
		philo->data->current_time = get_time();
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", philo->data->current_time, philo->id);
		pthread_mutex_unlock(&philo->write_mutex);
		pthread_mutex_lock(philo->left_fork);
		philo->data->forks_available[philo->id - 1] = 1;
		philo->data->current_time = get_time();
		pthread_mutex_lock(&philo->write_mutex);
		printf("%lld %d has taken a fork\n", philo->data->current_time, philo->id);
		pthread_mutex_unlock(&philo->write_mutex);
	}
	
}

void	eat(t_philo *philo)
{
	philo->state = 2;
	philo->data->current_time = get_time();
	pthread_mutex_lock(&philo->write_mutex);
	printf("%lld %d is eating\n", philo->data->current_time, philo->id);
	pthread_mutex_unlock(&philo->write_mutex);
	usleep(philo->data->time_eat * 1000);

	pthread_mutex_unlock(philo->left_fork);
	philo->data->forks_available[philo->id - 1] = 0;
	pthread_mutex_unlock(philo->right_fork);
	philo->data->forks_available[philo->id] = 0;
	philo->data->meals_eaten++;
	philo->data->last_meal_time = get_time();
}

void	sleep_and_think(t_philo *philo)
{
	
	philo->data->current_time = get_time();
	pthread_mutex_lock(&philo->write_mutex);
	printf("%lld %d is sleeping\n", philo->data->current_time, philo->id);
	pthread_mutex_unlock(&philo->write_mutex);
	usleep(philo->data->time_sleep * 1000);
	philo->state = 0;
	
	philo->data->current_time = get_time();
	pthread_mutex_lock(&philo->write_mutex);
	printf("%lld %d is thinking\n", philo->data->current_time, philo->id);
	pthread_mutex_unlock(&philo->write_mutex);
	
}

/* void    waiter(t_philo *philo)
{
	if(philo->data->fork_available[philo->id][0] == 0 && philo->data->fork_available[philo->id][1] == 0)
	{
		eat(philo);
		philo->data->fork_available[philo->id][0] = 1;
		philo->data->fork_available[philo->id][1] = 1;
		return ;
	}
	else
		take_forks(philo);
} */