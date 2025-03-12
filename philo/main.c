/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:47:46 by cle-berr          #+#    #+#             */
/*   Updated: 2025/03/12 10:19:39 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].meals_eaten = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
	}
}

int	init_data(t_data *data, char **argv, int argc)
{
	data->num_philos = ft_atol(argv[1]);
	if (data->num_philos > 200 || data->num_philos <= 0)
		return (printf(RED "num_philo must be between 1 and \
200\n" RESET), 0);
	data->time_to_die = ft_atol(argv[2]);
	if (data->time_to_die > INT_MAX || data->time_to_die <= 0)
		return (printf(RED "time_to_die must be between 1 \
and INT_MAX\n" RESET), 0);
	data->time_to_eat = ft_atol(argv[3]);
	if (data->time_to_eat > INT_MAX || data->time_to_eat <= 0)
		return (printf(RED "time_to_eat must be between 1 \
and INT_MAX\n" RESET), 0);
	data->time_to_sleep = ft_atol(argv[4]);
	if (data->time_to_sleep > INT_MAX || data->time_to_sleep <= 0)
		return (printf(RED "time_to_sleep must be between 1 \
and INT_MAX\n" RESET), 0);
	if (argc == 6)
		data->must_eat = ft_atol(argv[5]);
	else
		data->must_eat = -1;
	data->dead = 0;
	data->start_time = get_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	return (1);
}

int	init_simu(t_data *data, int argc, char **argv)
{
	int	i;

	i = -1;
	if (argc == 6)
		if (ft_atol(argv[5]) > INT_MAX || ft_atol(argv[5]) < 0)
			return (printf(RED "must_eat must be between 0 \
and INT_MAX\n" RESET), 0);
	if (!init_data(data, argv, argc))
		return (0);
	if (!data->forks || !data->philos)
		return (0);
	pthread_mutex_init(&data->thread, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	while (++i < data->num_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	init_philo(data);
	return (1);
}

void	start_simu(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
		pthread_create(&data->philos[i].thread, NULL, philo_routine,
			&data->philos[i]);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(data->philos[i].thread, NULL);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (printf("Usage: ./philo num_philos time_to_die \
time_to_eat time_to_sleep [num_eat]\n"), 1);
	if (!init_simu(&data, argc, argv))
		return (1);
	if (data.num_philos == 1)
		return (printf(YELLOW "%lld 1 has taken a fork\n" RESET, get_time()
				- data.start_time), usleep(data.time_to_die * 1000),
			printf(RED "%lld 1 died\n" RESET, get_time()
				- data.start_time),
			free(data.forks), 0);
	start_simu(&data);
	cleanup(&data);
	return (0);
}
