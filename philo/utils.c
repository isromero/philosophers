/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:50:34 by isromero          #+#    #+#             */
/*   Updated: 2023/04/27 09:50:34 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_data(t_data *data)
{
	data->n_philo = 1;
	data->id = 1;
	data->time_die = 0;
	data->time_eat = 0;
	data->time_sleep = 0;
	data->time_think = 0;
}

int	ft_atoi(const char *str)
{
	int					p;
	int					n;
	unsigned int		res;

	p = 0;
	n = 1;
	res = 0;
	while ((str[p] >= 9 && str[p] <= 13) || (str[p] == 32))
		p++;
	if (str[p] == '+' || str[p] == '-')
	{
		if (str[p] == '-')
			n = n * -1;
		p++;
	}
	while (str[p] >= '0' && str[p] <= '9')
	{
		res = (str[p] - '0') + (res * 10);
		p++;
	}
	if (res > 2147483648 && n == -1)
		return (0);
	if (res > 2147483647 && n == 1)
		return (-1);
	return (res * n);
}