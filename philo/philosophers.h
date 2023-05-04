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

typedef struct	s_shared
{
	int					n_philo;
	int					n_meals;
	int					meals_eaten;
	
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
}	t_shared;

typedef struct	s_philo
{
	pthread_t			thread;
	int					id;
	long long			start_time;
	long long			last_meal_time;
	int					sim_stop;
	
	pthread_mutex_t		fork;
	pthread_mutex_t		last_meal_time_access;
	pthread_mutex_t		write;
	pthread_mutex_t		meals;
	pthread_mutex_t		stop;

	t_shared			*shared;
	struct s_philo 		*next;
}	t_philo;

/* philosophers.c */
void	*routine(void *philo_data);
int		parse_args(int argc, char **argv, t_shared *shared);

/* actions.c */
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	sleep_and_think(t_philo *philo);

/* utils */
long long	get_time(void);
t_philo		*node_add_back(t_philo **lst);
int			ft_atoi(const char *str);

/* data	*/
void		init_data_philo(t_philo *philo);
void		free_data(t_philo *philo);

#endif