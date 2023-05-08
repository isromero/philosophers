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
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		log_message(philo, FORK);

		pthread_mutex_lock(philo->left_fork);
		log_message(philo, FORK);
	}
}
		
	
void	eat(t_philo *philo)
{
	philo->last_meal_time = get_time();

	log_message(philo, EAT);
	usleep(philo->args->time_to_eat * 1000);

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_and_think(t_philo *philo)
{
	log_message(philo, SLEEP);
	usleep(philo->args->time_to_sleep * 1000);
	log_message(philo, THINK);
}
