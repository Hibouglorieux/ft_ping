/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nallani <nallani@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:52:30 by nallani           #+#    #+#             */
/*   Updated: 2022/05/06 19:31:46 by nallani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	test(int signum)
{
	printf("%d!\n", signum);
	exit(42);
}

int		main(int argc, char** argv)
{
	s_ping_data ping_data;
	ft_memset(&ping_data, 0, sizeof(ping_data));

	if (!parse_arguments(argc, argv, &(ping_data.args)))
		return 1;
	if (ping_data.args.option_h)
	{
		fprintf(stderr, USAGE_STRING);
		return 0;
	}
	if (!verify_address(&ping_data))
		return 1;
	signal(SIGINT, test);
	ping_data.socketfd = create_socket();
	if (ping_data.socketfd < 0)
	{
		printf("sendto ret : %d %s\n", ping_data.socketfd, strerror(errno));
		return 1;
	}
	if (!send_packet(&ping_data))
		return 1;
	freeaddrinfo(ping_data.addrinfo);

	return 0;
}
