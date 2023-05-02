/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:32:48 by isromero          #+#    #+#             */
/*   Updated: 2023/04/27 09:32:48 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*check_philosophers(void* arg)
{
	t_philo	*philo = (t_philo *)arg;
	while(1)
	{
		if(get_time() - philo->last_meal_time >= philo->shared->time_to_die)
		{
			printf("%lldms %d died\n", get_time(), philo->id);
			free_data(philo);
			return (0);
		}
	}
	return (0);
}

void	*routine(void *philo_data)
{
	t_philo	*philo;

	philo = (t_philo *)philo_data;

	while(1)
	{
		take_forks(philo);
		break ;
	}
	return 0;
}

int	parse_args(int argc, char **argv, t_shared *shared)
{
	if(argc == 5 || argc == 6)
	{
		shared->n_philo = ft_atoi(argv[1]);
		shared->time_to_die = ft_atoi(argv[2]);
		shared->time_to_eat = ft_atoi(argv[3]);
		shared->time_to_sleep = ft_atoi(argv[4]);
		if(argc == 6)
		{
			shared->n_meals = ft_atoi(argv[5]);
			if(shared->n_meals <= 0)
			{
				printf("Invalid arguments\n");
				return(0);
			}
		}
		if(shared->n_philo <= 0 || shared->time_to_die <= 0 || shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
		{
			printf("Invalid arguments\n");
			return(0);
		}
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_shared 	shared;
	t_philo		*philo;
	t_philo		*last_philo;
	t_philo		*new_philo;
	pthread_t 	check_thread;
	int			i;

	if(!parse_args(argc, argv, &shared))
		return (1);
	
	philo = NULL;
	last_philo = NULL;
	new_philo = NULL;
	i = 0;
	while(i < shared.n_philo)
	{
		new_philo = add_node_to_cllist(&philo);
		init_data_philo(new_philo);
		new_philo->id = i + 1;
		new_philo->shared = &shared;
		pthread_create(&new_philo->thread, NULL, &routine, new_philo);
		last_philo = new_philo;
		i++;
	}
	last_philo->next = philo;
	pthread_create(&check_thread, NULL, &check_philosophers, philo);

	i = 0;
	while(i < shared.n_philo)
	{
		pthread_join(philo->thread, NULL);
		philo = philo->next;
		i++;
	}
	t_philo *tmp;
	while (philo)
	{
		tmp = philo->next;
		pthread_mutex_destroy(&philo->died);
		pthread_mutex_destroy(&philo->meals);
		pthread_mutex_destroy(&philo->last_meal_time_access);
		pthread_mutex_destroy(&philo->fork);
		free(philo);
		philo = tmp;
	}
	pthread_join(check_thread, NULL);
	return (0);
}