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

/* This function takes care of a philosopher's attempt to pick up the forks on their left and right sides to eat.
It first checks if the philosopher's ID is odd or even. If it's odd, the philosopher first tries to pick up the 
fork on their left and then the one on their right. If it's even, they first try to pick up the fork on their right 
and then the one on their left. This order helps prevent a deadlock condition where all philosophers pick up a fork 
on one side simultaneously and are then unable to pick up the fork on the other side because they are all already taken.
For each successful acquisition of a fork, the function logs the event using the log_message function.*/
void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		log_message(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		log_message(philo, FORK);
	}
	else if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		log_message(philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		log_message(philo, FORK);
	}
}

/* 
This function represents the process of a philosopher eating. First, the variable that updates the last time you have eaten
is locked, always before you start eating. It makes usleep for the set amount of time and unlocks the two forks it has used. 
Once this is done, the variable of the times eaten is updated. */
void	eat(t_philo *philo)
{
	long long int	end_time;

	pthread_mutex_lock(&philo->args->lock_last_meal_time);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->args->lock_last_meal_time);
	log_message(philo, EAT);
	end_time = get_time() + philo->time_to_eat;
	while (get_time() < end_time)
		usleep(50);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->args->lock_meals_eaten);
	philo->args->meals_eaten++;
	pthread_mutex_unlock(&philo->args->lock_meals_eaten);
}

/* This function represents the action of sleeping and thinking for a philosopher. Just print both logs and usleep for the 
specified time_to_sleep */
void	sleep_and_think(t_philo *philo)
{
	long long int	end_time;

	end_time = get_time() + philo->time_to_sleep;
	log_message(philo, SLEEP);

	while (get_time() < end_time)
		usleep(50);
	log_message(philo, THINK);
}
