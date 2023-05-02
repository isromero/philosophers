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
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>

typedef struct	s_data
{
	int	n_philo;
	long long	time_die;
	long long	time_eat;
	long long	time_sleep;
	long long	time_think;
	long long	start_time;
	long long	current_time;
	long long	last_meal_time;
	long long	last_fork_time;
	int		n_meals;
	int		sim_stop;
	int		meals_eaten;
	int		*forks_available;
}	t_data;

typedef struct	s_philo
{
	int	id;
	int	state;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t write_mutex;
	pthread_mutex_t sim_stop;
	pthread_mutex_t meal_time;
	pthread_mutex_t meals_eaten_mutex;
	t_data	*data;
} t_philo;

/* philosophers.c */
void	*routine(void *philo_data);
int		parse_args(int argc, char **argv, t_data *data);

/* actions.c */
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	sleep_and_think(t_philo *philo);

/* utils */
int			ft_atoi(const char *str);
long long	get_time(void);

#endif