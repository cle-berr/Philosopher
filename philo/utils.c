/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:53:42 by cle-berr          #+#    #+#             */
/*   Updated: 2025/03/12 11:56:15 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	sleep_ms(long ms, t_data *data, t_philo *philo)
{
	long	start;

	start = get_time();
	while (get_time() - start < ms)
	{
		if (check_death(philo, data))
			return (0);
		usleep(500);
	}
	return (1);
}

void	print_action(t_data *data, int id, char *msg, char *color)
{
	pthread_mutex_lock(&data->print_lock);
	printf("%s%lld %d %s%s\n" RESET, color, get_time() - data->start_time,
			id, msg, RESET);
	pthread_mutex_unlock(&data->print_lock);
}

int	check_death(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->thread);
	if (data->dead)
	{
		pthread_mutex_unlock(&data->thread);
		return (1);
	}
	if (get_time() - philo->last_meal >= data->time_to_die)
	{
		print_action(data, philo->id, "died", RED);
		data->dead = 1;
		pthread_mutex_unlock(&data->thread);
		return (1);
	}
	pthread_mutex_unlock(&data->thread);
	return (0);
}

void	cleanup(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->print_lock);
	free(data->forks);
	free(data->philos);
}
