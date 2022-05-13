/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nallani <unkown@noaddress.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 17:07:32 by nallani           #+#    #+#             */
/*   Updated: 2022/05/06 19:26:00 by nallani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

size_t	ft_strlen(const char* str)
{
	int i = 0;

	if (!str)
		return 0;
	while (str[i])
	{
		i++;
	}
	return i;
}

void	ft_memset(void* addr,char value, size_t size)
{
	char* addr_as_char = (char*)addr;
	for (size_t i = 0; i < size; i++)
	{
		addr_as_char[i] = value;
	}
}

void	ft_memcpy(void* dst,const void* src, size_t size)
{
	char* dst_char = (char*)dst;
	const char* src_char = (char*)src;

	for (size_t i = 0; i < size; i++)
	{
		dst_char[i] = src_char[i];
	}
}
