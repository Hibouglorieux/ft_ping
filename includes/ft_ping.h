/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nallani <nallani@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:54:50 by nallani           #+#    #+#             */
/*   Updated: 2022/05/06 17:27:18 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FT_PING_CLASS_H
# define FT_PING_CLASS_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <linux/icmp.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>// TOREMOVE
#include <linux/ip.h>

#include <netinet/in.h> // needed for inet_aton
#include <arpa/inet.h>

#define USAGE_STRING "\nUsage\n  ft_ping [options] <destination>\nOptions:\n <destination>\t\tdns name or ip address\n"\
						"  -h\t\t\tprint help and exit\n  -v\t\t\tverbose output\n"

typedef struct	arguments
{
	char*	address;
	bool 	option_v;
	bool	option_h;
}				s_arguments;

typedef struct	ping_data
{
	s_arguments 		args;
	struct addrinfo*	addrinfo;
	int					socketfd;
}				s_ping_data;

/*
 * parser.c
 */

bool	parse_arguments(int argc, char** argv, s_arguments* args);


/*
 * utilities.c
 */

size_t	ft_strlen(const char* str);
void	ft_memset(void* addr,char value, size_t size);
void	ft_memcpy(void* dst,const void* src, size_t size);

/*
 * ft_ping.c
 */

bool	verify_address(s_ping_data* ping_data);
bool	send_packet(s_ping_data* args);
int		create_socket();
#endif
