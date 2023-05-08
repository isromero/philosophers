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
	if (argc < 5)
	{
		printf("Invalid arguments\n");
		return (EXIT_FAILURE);
	}
	args->n_philos = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	pthread_mutex_init(&args->lock_print, NULL);
	pthread_mutex_init(&args->lock_death, NULL);
	if (argc == 6)
	{
		args->n_meals = ft_atoi(argv[5]);
		if (args->n_meals <= 0)
		{
			printf("Invalid arguments\n");
			return (EXIT_FAILURE);
		}
	}
	if (args->n_philos <= 0 || args->time_to_die <= 0 
  	|| args->time_to_eat <= 0 || args->time_to_sleep <= 0)
	{
		printf("Invalid arguments\n");
		return (EXIT_FAILURE);
	}
	args->stop_sim = false;
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

void	init_philos(t_args *args)
{
	t_philo		*philos;
	int			i;

	philos = malloc(sizeof(t_philo) * args->n_philos);
	philos->threads = malloc(sizeof(pthread_t) * args->n_philos);
	i = 0;
	while (i < args->n_philos)
	{
		philos[i].id = i + 1;
		philos[i].last_meal_time = get_time();
		philos[i].is_eating = 0;
		philos[i].left_fork = &args->forks[i];
		philos[i].right_fork = &args->forks[(i + 1) % args->n_philos];
		philos[i].args = args;
		pthread_create(&philos->threads[i], NULL, routine, &philos[i]);
		i++;
	}

	i = 0;
	while (i < philos->args->n_philos)
	{
		pthread_join(philos->threads[i], NULL);
		pthread_join(philos[i].check, NULL);
		i++;
	}

	i = 0;
	while (i < philos->args->n_philos)
	{
 		pthread_mutex_destroy(&args->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&args->lock_print);
	pthread_mutex_destroy(&args->lock_death);
	free(philos->threads);
	free(philos);
	free(args->forks);
}
