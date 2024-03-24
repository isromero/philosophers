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
	init_args(argc, argv, args);
	if (argc == 6)
	{
		args->n_meals = ft_atoi(argv[5]);
		if (args->n_meals <= 0)
		{
			printf("Invalid arguments\n");
			return (EXIT_FAILURE);
		}
	}
	if (argc < 5 || argc > 6 || args->n_philos <= 0 || args->time_to_die <= 0 \
	|| args->time_to_eat <= 0 || args->time_to_sleep <= 0)
	{
		printf("Invalid arguments\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	init_forks(t_args *args)
{
	int	i;

	args->forks = malloc(sizeof(t_args) * args->n_philos);
	i = 0;
	while (i < args->n_philos)
	{
		pthread_mutex_init(&args->forks[i], NULL);
		i++;
	}
}

void	init_philos_and_mutexes(t_philo *philos, t_args *args)
{
	int	i;

	init_mutexes(args);
	i = 0;
	while (i < args->n_philos)
	{
		philos[i].id = i + 1;
		philos[i].last_meal_time = get_time();
		philos[i].n_philos = args->n_philos;
		philos[i].n_meals = args->n_meals;
		philos[i].time_to_die = args->time_to_die;
		philos[i].time_to_eat = args->time_to_eat;
		philos[i].time_to_sleep = args->time_to_sleep;
		philos[i].left_fork = &args->forks[i];
		philos[i].right_fork = &args->forks[(i + 1) % args->n_philos];
		philos[i].args = args;
		pthread_create(&philos[i].thread, NULL, routine, &philos[i]);
		pthread_create(&philos[i].death_check, NULL, check_death, &philos[i]);
		pthread_create(&philos[i].meals_check, NULL, check_meals, &philos[i]);
		i++;
	}
}

void	join_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->args->n_philos)
	{
		pthread_join(philos[i].thread, NULL);
		pthread_join(philos[i].death_check, NULL);
		pthread_join(philos[i].meals_check, NULL);
		i++;
	}
}

void	free_and_destroy(t_philo *philos, t_args *args)
{
	int	i;

	i = 0;
	while (i < philos->args->n_philos)
	{
		pthread_mutex_destroy(&args->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&args->lock_print);
	pthread_mutex_destroy(&args->lock_death);
	pthread_mutex_destroy(&args->lock_last_meal_time);
	pthread_mutex_destroy(&args->lock_meals_eaten);
	pthread_mutex_destroy(&args->lock_meals_stop);
	pthread_mutex_destroy(&args->lock_stop_sim);
	free(philos);
	free(args->forks);
}
