/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nallani <unkown@noaddress.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:57:34 by nallani           #+#    #+#             */
/*   Updated: 2022/05/06 18:08:15 by nallani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

bool	handle_arg(char* arg, s_arguments* args)
{
	size_t length = ft_strlen(arg);

	// handle options
	if (arg[0] == '-' && length > 1)
	{
		for (size_t i = 1; i < length; i++)
		{
			if (arg[i] == 'v')
				args->option_v = true;
			else if (arg[i] == 'h')
			{
				args->option_h = true;
				return true;
			}
			else
			{
				fprintf(stderr, "ft_ping: invalid option -- '%c'\n", arg[i]);
				return false;
			}
		}
	}
	// handle address
	else if (args->address == NULL)
		args->address = arg;
	return true;
}

bool	are_args_valid(s_arguments* args)
{
	if (args->option_h)
		return true;
	if (args->address == NULL)
	{
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		return false;
	}
	return true;
}

bool	parse_arguments(int argc, char** argv, s_arguments* args)
{
	if (argc < 2)
	{
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		return false;
	}
	for (int i = 1; i < argc; i++)
	{
		if (!handle_arg(argv[i], args))
			return false;
	}
	return are_args_valid(args);
}
