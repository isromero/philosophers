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
#include <pthread.h>

typedef struct	s_data {
	int	n_philo;
	int	id;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	time_think;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
}	t_data;

/* philosophers.c */
void	*routine(void *philo_data);

/* actions.c */
void	take_forks(t_data *philo);

/* utils */
int		ft_atoi(const char *str);
void	init_data(t_data *data);

#endif