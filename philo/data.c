/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 20:36:14 by isromero          #+#    #+#             */
/*   Updated: 2023/05/02 20:36:14 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_data_philo(t_philo *philo)
{
	philo->start_time = 0;
	
	philo->last_meal_time = 0;
	philo->sim_stop = 0;
	pthread_mutex_init(&philo->stop, NULL);
	pthread_mutex_init(&philo->meals, NULL);
	pthread_mutex_init(&philo->last_meal_time_access, NULL);
	pthread_mutex_init(&philo->fork, NULL);
}

void	free_data(t_philo *philo)
{
	pthread_mutex_destroy(&philo->stop);
	pthread_mutex_destroy(&philo->meals);
	pthread_mutex_destroy(&philo->last_meal_time_access);
	pthread_mutex_destroy(&philo->fork);
}