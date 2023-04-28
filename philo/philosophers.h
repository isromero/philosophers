/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:33:15 by isromero          #+#    #+#             */
/*   Updated: 2023/04/27 09:33:15 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct	s_data
{
	int	n_philo;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	time_think;
	unsigned long long	start_time;
	unsigned long long	current_time;
	int	n_meals;
}	t_data;

typedef struct	s_philo
{
	int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data	*data;
} t_philo;

/* philosophers.c */
void	*routine(void *philo_data);

/* actions.c */
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);

/* utils */
int		ft_atoi(const char *str);
unsigned long long	get_time(void);

#endif