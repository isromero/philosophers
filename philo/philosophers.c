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
			free_data(philo);
			printf("%lldms %d died\n", get_time(), philo->id);
			pthread_mutex_lock(&philo->stop);
			philo->sim_stop = 1;
			pthread_mutex_unlock(&philo->stop);
			return (0);
		}
		if(philo->shared->n_meals > 0 && (philo->shared->meals_eaten >= philo->shared->n_meals * philo->shared->n_philo))
		{
			free_data(philo);
			pthread_mutex_lock(&philo->stop);
			philo->sim_stop = 1;
			pthread_mutex_unlock(&philo->stop);
			free_data(philo);
			return(0);
		}
		
		usleep(1000);
	}
	return (0);
}

void	*routine(void *philo_data)
{
	t_philo	*philo;

	philo = (t_philo *)philo_data;
	philo->shared->meals_eaten = 0;
	while(1)
	{
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
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
	t_philo		*first_philo;
	pthread_t 	check_thread;
	int			i;

	if(!parse_args(argc, argv, &shared))
		return (1);
	
	philo = NULL;
	last_philo = NULL;
	first_philo = NULL;
	
	i = 0;
	while(i < shared.n_philo)
	{
		philo = node_add_back(&philo);
		init_data_philo(philo);
		philo->id = i + 1;
		philo->shared = &shared;
		pthread_create(&philo->thread, NULL, &routine, philo);
		pthread_detach(philo->thread); ///////////////////
		if (i == 0)
			first_philo = philo;
		last_philo = philo;
		i++;
	}
	last_philo->next = first_philo;
	
	pthread_create(&check_thread, NULL, &check_philosophers, philo);
	
	i = 0;
	while(i < shared.n_philo)
	{
		pthread_join(philo->thread, NULL);
		philo = philo->next;
		i++;
	}
	free_data(philo);
	pthread_join(check_thread, NULL);
	return (0);
}