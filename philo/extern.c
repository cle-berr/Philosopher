/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extern.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:20:00 by cle-berr          #+#    #+#             */
/*   Updated: 2025/03/13 10:26:36 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *nptr)
{
	long	i;
	long	result;
	long	sign;

	i = 0;
	result = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	while (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		if (nptr[i + 1] == '-' || nptr[i +1] == '+')
			return (0);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

int	ft_strdigit(char **s)
{
	int	i;
	int	j;

	j = 1;
	while (s[j])
	{
		i = 0;
		while (s[j][i])
		{
			if (s[j][i] < '0' || s[j][i] > '9')
				return (printf(RED "{%s} must contain only digit" RESET, s[j]),
					0);
			i++;
		}
		j++;
	}
	return (1);
}
