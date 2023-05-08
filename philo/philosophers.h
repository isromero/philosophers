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
	pthread_mutex_t		lock_meals_stop;
	pthread_mutex_t		lock_meals_eaten;
	pthread_mutex_t		lock_last_meal_time;

	bool				stop_sim;
}	t_args;

typedef struct	s_philo
{
	int					id;
	long long			last_meal_time;
	pthread_t			*threads;
	pthread_t			death_check;
	pthread_t			meals_check;

	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_args				*args;
}	t_philo;

typedef enum e_state {
	DEAD,
	EAT,
	THINK,
	SLEEP,
	FORK,
	DROP,
}	t_state;

#define TAKE_FORK_STR "has taken a fork\n"
#define EAT_STR "is eating\n"
#define THINK_STR "is thinking\n"
#define SLEEP_STR "is sleeping\n"
#define DEAD_STR "is dead\n"

/* philosophers.c */
void	*routine(void *args);
void	*check_death(void *args);
void	*check_meals(void *args);

/* actions.c */
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	sleep_and_think(t_philo *philo);

/* utils */
long long	get_time(void);
int			ft_atoi(const char *str);
void		log_message(t_philo *philo, t_state state);

/* data	*/
int		parse_args(int argc, char **argv, t_args *args);
void	init_philos(t_args *args);
void	init_forks(t_args *args);

#endif