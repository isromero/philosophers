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

typedef struct	s_args
{
	int					n_philos;
	int					n_meals;
	int					meals_eaten;

	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;

	pthread_mutex_t		*forks;
	pthread_mutex_t		lock_print;
	pthread_mutex_t		lock_death;
}	t_args;

typedef struct	s_philo
{
	int					id;
	int					n_philos;
	long long			last_meal_time;
	int					is_eating;
	int					stop;
	pthread_t			*threads;
	pthread_t			check;

	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;

	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_args				args;

}	t_philo;

/* philosophers.c */
void	*routine(void *args);
void	*check_philosophers(void *args);

/* actions.c */
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	sleep_and_think(t_philo *philo);

/* utils */
long long	get_time(void);
int			ft_atoi(const char *str);

/* data	*/
int		parse_args(int argc, char **argv, t_args *args);
void	init_philos(t_args *args);
void	init_forks(t_args *args);
void	join_threads(t_args *args, t_philo *philo);

#endif