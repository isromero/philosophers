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

int	parse_args(int argc, char **argv, t_args *args)
{
	if(argc == 5 || argc == 6)
	{
		args->n_philos = ft_atoi(argv[1]);
		args->time_to_die = ft_atoi(argv[2]);
		args->time_to_eat = ft_atoi(argv[3]);
		args->time_to_sleep = ft_atoi(argv[4]);
		pthread_mutex_init(&args->lock_print, NULL);
		pthread_mutex_init(&args->lock_death, NULL);
		if(argc == 6)
		{
			args->n_meals = ft_atoi(argv[5]);
			if(args->n_meals <= 0)
			{
				printf("Invalid arguments\n");
				return(0);
			}
		}
		if(args->n_philos <= 0 || args->time_to_die <= 0 || args->time_to_eat <= 0 || args->time_to_sleep <= 0)
		{
			printf("Invalid arguments\n");
			return(0);
		}
	}
	return (1);
}

void	init_forks(t_args *args)
{
	int	i;

	i = 0;
	args->forks = malloc(sizeof(t_args) * args->n_philos);
	while  (i < args->n_philos) 
	{
    	pthread_mutex_init(&args->forks[i], NULL);
		i++;
	}
}

void	init_philos(t_args *args)
{
	t_philo		*philo;
	pthread_t	*threads;
	int			i;

	philo = malloc(sizeof(t_philo) * args->n_philos);
	threads = malloc(sizeof(t_philo) * args->n_philos);
	i = 0;
	
	while(i < args->n_philos && philo->stop == 0)
	{
		philo[i].id = i + 1;
		philo[i].last_meal_time = 0;
		philo[i].n_philos = args->n_philos;
		philo[i].is_eating = 0;
		philo[i].stop = 0;
		philo[i].time_to_die = args->time_to_die;
		philo[i].time_to_eat = args->time_to_eat;
		philo[i].time_to_sleep = args->time_to_sleep;
		philo[i].left_fork = &args->forks[i];
		philo[i].right_fork = &args->forks[(i + 1) % args->n_philos];
		pthread_create(&threads[i], NULL, routine, &philo[i]);
		i++;
	}
	i = 0;
	while(i < philo->n_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(args->forks);
	free(threads);
	free(philo);
	free(args->forks);
}

//problema con mandar la señal de muerte, los returns no hacen que termine el programa porque no se le indica bien
