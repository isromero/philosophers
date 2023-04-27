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

void	take_forks(t_data *philo)
{
	printf("[%d] is trying to take forks\n", philo->id);

	//lock the left fork
	if (pthread_mutex_lock(&philo->left_fork) == -1) 
	{
        printf("[%d] failed to take left fork\n", philo->id);
        return ;
    }
    printf("[%d] took left fork\n", philo->id);

	//lock the right fork
    if (pthread_mutex_lock(&philo->right_fork) == -1) 
	{
        //failed to lock right fork, release the left fork and return
        pthread_mutex_unlock(&philo->left_fork);
        printf("[%d] failed to take right fork\n", philo->id);
        return ;
    }
    printf("[%d] took right fork\n", philo->id);
}

void	eat(t_data *philo)
{
	printf("%llu %d is eating\n", philo->current_time, philo->id);
	usleep(philo->time_eat * 1000);
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(&philo->right_fork);
}
