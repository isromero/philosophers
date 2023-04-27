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
	int		id;

	//the thread routine, we cast the void*
	philo = (t_data *)philo_data;
	id = data->id;
	while(1)
	{
		take_forks(philo);
		eat(philo);
		leave_forks(philo);
		sleep(philo);
		think(philo);
	}
}

int	main(int argc, char **argv)
{
	t_data 		data;
	pthread_t	*threads;
	int	i;

	init_data(&data);

	if(argc > 1) 
	{
		i = 0;
		data.n_philo = ft_atoi(argv[1]);
		threads = malloc(sizeof(pthread_t) * data.n_philo);
		while(i < data.n_philo)
		{
			//number of threads created are the philo passed in argv[1]
			if (pthread_create(&threads[i], NULL, &routine, &data) != 0) 
			{
				printf("Error creating thread\n");
				exit(1);
			}
			i++;
		}
		//wait until all threads finished
		i = 0;
		//i is incremented first of all, for start with thread 1
		while(++i < data.n_philo)
			pthread_join(threads[i], NULL);
	}
	free(threads);
	/* pthread_mutex_t	fork_mutex;

	pthread_mutex_init(&fork_mutex, NULL);

	pthread_mutex_lock(&fork_mutex);

	pthread_mutex_unlock(&fork_mutex);

	pthread_mutex_destroy(&fork_mutex); */
	return (0);
}