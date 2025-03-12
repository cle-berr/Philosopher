/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:12:01 by cle-berr          #+#    #+#             */
/*   Updated: 2025/03/12 11:58:21 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_fork(t_data *data, t_philo *philo)
{
	if (check_death(philo, data))
		return (0);
	pthread_mutex_lock(philo->left_fork);
	if (check_death(philo, data))
		return (pthread_mutex_unlock(philo->left_fork), 0);
	print_action(data, philo->id, "has taken a fork", YELLOW);
	if (check_death(philo, data))
		return (pthread_mutex_unlock(philo->left_fork), 0);
	pthread_mutex_lock(philo->right_fork);
	if (check_death(philo, data))
		return (pthread_mutex_unlock(philo->left_fork),
			pthread_mutex_unlock(philo->right_fork), 0);
	print_action(data, philo->id, "has taken a fork", YELLOW);
	if (check_death(philo, data))
		return (pthread_mutex_unlock(philo->left_fork),
			pthread_mutex_unlock(philo->right_fork), 0);
	return (1);
}

int	eating(t_data *data, t_philo *philo)
{
	print_action(data, philo->id, "is eating", GREEN);
	if (check_death(philo, data))
		return (pthread_mutex_unlock(philo->left_fork),
			pthread_mutex_unlock(philo->right_fork), 0);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	if (!sleep_ms(data->time_to_eat, data, philo))
		return (pthread_mutex_unlock(philo->left_fork),
			pthread_mutex_unlock(philo->right_fork), 0);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (1);
}

int	sleep_and_think(t_data *data, t_philo *philo)
{
	print_action(data, philo->id, "is sleeping", BLUE);
	if (!sleep_ms(data->time_to_sleep, data, philo))
		return (0);
	if (check_death(philo, data))
		return (0);
	print_action(data, philo->id, "is thinking", MAGENTA);
	return (1);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	t_data			*data;
	pthread_mutex_t	*tmp;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 != 0)
	{
		tmp = philo->right_fork;
		philo->right_fork = philo->left_fork;
		philo->left_fork = tmp;
	}
	while (philo->meals_eaten != data->must_eat)
	{
		if (!take_fork(data, philo))
			return (NULL);
		if (!eating(data, philo))
			return (NULL);
		if (check_death(philo, data))
			return (NULL);
		if (!sleep_and_think(data, philo))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
