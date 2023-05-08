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

/* This function represents the main routine of each philosopher thread in the dining philosophers problem 
simulation. It creates two threads, one to check for the philosopher's death, and the other to check for the 
maximum number of meals eaten. It then enters a loop where the philosopher alternates between taking two 
forks, eating, and sleeping/thinking. The loop continues until the simulation is stopped by the main thread. 
If there is only one philosopher, the philosopher takes the fork and immediately returns. */
void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	pthread_create(&philo->death_check, NULL, check_death, philo);
	pthread_create(&philo->meals_check, NULL, check_meals, philo);
	while (!philo->args->stop_sim)
	{
		if (philo->args->n_philos == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			log_message(philo, FORK);
			return (NULL);
		}
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_args	args;
	t_philo	*philos;

	if (parse_args(argc, argv, &args) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	philos = malloc(sizeof(t_philo) * args.n_philos);
	if (!philos)
		return (EXIT_FAILURE);
	init_forks(&args);
	init_philos_and_mutexes(philos, &args);
	join_philos(philos);
	free_and_destroy(philos, &args);
	return (EXIT_SUCCESS);
}
