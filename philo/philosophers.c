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

void	*routine(void *philo_data)
{
	t_data	*philo;

	//the thread routine, we cast the void*
	philo = (t_data *)philo_data;
	while(1)
	{
		take_forks(philo);
		/* eat(philo); */
		/*leave_forks(philo);
		sleep(philo);
		think(philo); */
	}
}

void	parse_args(int argc, char **argv, t_data *data)
{
	int	i;

	i = 0;
	if(argc == 5 || argc == 6)
	{
		data->n_philo = ft_atoi(argv[1]);
		while(i++ < data->n_philo)
			data[i].id = i + 1;
		data->time_die = ft_atoi(argv[2]);
		data->time_eat = ft_atoi(argv[3]);
		data->time_sleep = ft_atoi(argv[4]);
		if(argc == 6)
			data->n_meals = ft_atoi(argv[5]);
		if(data->n_philo <= 0 || data->time_die <= 0 || data->time_eat <= 0 || data->time_sleep <= 0)
			printf("Invalid arguments\n");
	}
	else
		printf("Invalid arguments\n");
}

int	main(int argc, char **argv)
{
	t_data 		data;
	pthread_t	*threads;
	int	i;

	parse_args(argc, argv, &data);
	i = 0;
	threads = malloc(sizeof(pthread_t) * data.n_philo);

	//mutex initialized
	pthread_mutex_init(&data.left_fork, NULL);
	pthread_mutex_init(&data.right_fork, NULL);
	while(i < data.n_philo)
	{
		data.id = i;
		//number of threads created are the philo passed in argv[1]
		if (pthread_create(&threads[i], NULL, &routine, &data) != 0) 
		{
			printf("Error creating thread\n");
			exit(1);
		}
		i++;
	}
	i = 0;
	//wait until all threads finished
	//i is incremented first of all, for start with thread 1
	while(++i < data.n_philo)
		pthread_join(threads[i], NULL);

	pthread_mutex_destroy(&data.left_fork);
    pthread_mutex_destroy(&data.right_fork);

	free(threads);
	return (0);
}