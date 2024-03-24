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

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(&philo->args->lock_stop_sim);
		if (philo->args->stop_sim)
		{
			pthread_mutex_unlock(&philo->args->lock_stop_sim);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->args->lock_stop_sim);
		if (philo->n_philos == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			log_message(philo, FORK);
			pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
		usleep(1000);
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
