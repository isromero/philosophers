/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 11:25:51 by isromero          #+#    #+#             */
/*   Updated: 2023/04/27 11:25:51 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_data *philo)
{
	pthread_mutex_loc(philo->left_fork);
	printf("%d\n", philo->id);
	printf("            ha cogido el tenedor izquierdo");
	pthread_mutex_loc(philo->right_fork);
	printf("%d\n", philo->id);
	printf("            ha cogido el tenedor derecho");
}
