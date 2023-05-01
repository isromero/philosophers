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

void* check_time_to_die(void* arg)
{
  	t_philo *philo = (t_philo *) arg;
  	while (1)
  	{
			pthread_mutex_lock(&philo->state_mutex);
      		if (philo->state != 2 && (get_time() - philo->data->last_meal_time) >= philo->data->time_die)
      		{
				pthread_mutex_unlock(&philo->state_mutex);
				pthread_mutex_unlock(&philo->meal_time);
        		// El filósofo ha muerto, establecer la variable de fin de simulación
				pthread_mutex_lock(&philo->sim_stop);
				philo->data->sim_stop = 1;
				pthread_mutex_unlock(&philo->sim_stop);
        		pthread_mutex_lock(&philo->write_mutex);
        		printf("%llu %d died\n", get_time(), philo->id);
				pthread_mutex_unlock(&philo->write_mutex);
       			exit(1); //can't use exit
      		}
			pthread_mutex_unlock(&philo->state_mutex);
			pthread_mutex_unlock(&philo->meal_time);
			// si se ha alcanzado el número de comidas, establecer la variable de fin de simulación
			pthread_mutex_lock(&philo->meals_eaten_mutex);
			if (philo->data->n_meals > 0 && philo->data->meals_eaten >= philo->data->n_philo * philo->data->n_meals)
			{
				pthread_mutex_unlock(&philo->meals_eaten_mutex);
				pthread_mutex_lock(&philo->sim_stop);
				philo->data->sim_stop = 1;
				pthread_mutex_unlock(&philo->sim_stop);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->meals_eaten_mutex);
		usleep(1000);
  	}
  return (0);
}

void	*routine(void *philo_data)
{
	t_philo	*philo;

	//the thread routine, we cast the void*
	philo = (t_philo *)philo_data;
	pthread_mutex_lock(&philo->meal_time);
	philo->data->last_meal_time = 0;
	pthread_mutex_unlock(&philo->meal_time);
	while(1)
	{
		pthread_mutex_lock(&philo->sim_stop);
		if(philo->data->sim_stop == 1)
			return (0);
		pthread_mutex_unlock(&philo->sim_stop);
		pthread_mutex_lock(&philo->available_forks);
		if(philo->data->forks_available[philo->id - 1] == 0 && philo->data->forks_available[philo->id] == 0)
		{
			pthread_mutex_unlock(&philo->available_forks);
			take_forks(philo);
			pthread_mutex_lock(&philo->sim_stop);
			if(philo->data->sim_stop == 1)
				return (0);
			pthread_mutex_unlock(&philo->sim_stop);
			eat(philo);
		}
		pthread_mutex_lock(&philo->state_mutex);
		if(philo->state == 2)
			sleep_and_think(philo);
		pthread_mutex_unlock(&philo->state_mutex);
		pthread_mutex_lock(&philo->state_mutex);
	}
	return 0;
}

int	parse_args(int argc, char **argv, t_data *data)
{
	if(argc == 5 || argc == 6)
	{
		data->n_philo = ft_atoi(argv[1]);
		data->time_die = ft_atoi(argv[2]);
		data->time_eat = ft_atoi(argv[3]);
		data->time_sleep = ft_atoi(argv[4]);
		if(argc == 6)
		{
			data->n_meals = ft_atoi(argv[5]);
			if(data->n_meals <= 0)
			{
				printf("Invalid arguments\n");
				return(0);
			}
		}
		if(data->n_philo <= 0 || data->time_die <= 0 || data->time_eat <= 0 || data->time_sleep <= 0)
		{
			printf("Invalid arguments\n");
			return(0);
		}
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_data 		data;
	t_philo		*philo;
	pthread_t	*threads;
	pthread_mutex_t *forks;
	int	i;
	

	if(!parse_args(argc, argv, &data))
		return (1);
	threads = malloc(sizeof(pthread_t) * data.n_philo);
	forks = malloc(sizeof(pthread_mutex_t) * data.n_philo);
	philo = malloc(sizeof(t_philo) * data.n_philo);
	data.forks_available = malloc(sizeof(int) * data.n_philo);
	memset(data.forks_available, 0, sizeof(int) * data.n_philo);
	pthread_t death_check_thread;
	pthread_mutex_init(&philo->write_mutex, NULL);
	pthread_mutex_init(&philo->sim_stop, NULL);
	pthread_mutex_init(&philo->meal_time, NULL);
	pthread_mutex_init(&philo->state_mutex, NULL);
	pthread_mutex_init(&philo->available_forks, NULL);
	pthread_mutex_init(&philo->meals_eaten_mutex, NULL);

	i = -1;
	//mutex initialized
	while(++i < data.n_philo)
	{
		philo[i].id = i + 1;

		//we initialize all mutex
		pthread_mutex_init(&forks[i], NULL);
		//we initialize the left fork, i is the number of philo, so philo->id 1 = fork 1 in the left
		philo[i].left_fork = &forks[i];
		//we initialize the right fork, but i + 1 because is the number of his right so philo->id 1 + 1 = fork 2 
		//so fork 2 is left side of id 2 and right side of id 1. And we use % because when is the last philosopher,
		//for example id 5, i + 1 will not exist, because i + 1 is the left side of philo->id 1, because is a round table
		//so we need the % for take the right fork of the data.n_philo - 1, in this example: (4+1) % 5 = 0, so is the 
		//0 position of the array, so, if the left of id 1 is 1, the left of 5 is 0.
		philo[i].right_fork = &forks[(i + 1) % data.n_philo];
		//initialize all forks available
		philo[i].data = &data;
		//number of threads created are the philo passed in argv[1]
		if (pthread_create(&threads[i], NULL, &routine, &philo[i]) != 0) 
		{
			printf("Error creating thread\n");
			return (1);
		}
	}
	pthread_create(&death_check_thread, NULL, check_time_to_die, philo);

	i = -1;
	//wait until all threads finished
	while(++i < data.n_philo)
		pthread_join(threads[i], NULL);
	
	pthread_join(death_check_thread, NULL);
	i = -1;
	while(++i < data.n_philo)
		pthread_mutex_destroy(&forks[i]);
	pthread_mutex_destroy(&philo->write_mutex);
	pthread_mutex_destroy(&philo->sim_stop);
	pthread_mutex_destroy(&philo->meal_time);
	pthread_mutex_destroy(&philo->state_mutex);
	pthread_mutex_destroy(&philo->available_forks);
	pthread_mutex_destroy(&philo->meals_eaten_mutex);
	free(threads);
	free(forks);
	free(data.forks_available);
	return (0);
}