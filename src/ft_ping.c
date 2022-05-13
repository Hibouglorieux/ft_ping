/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nallani <nallani@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 17:35:48 by nallani           #+#    #+#             */
/*   Updated: 2022/05/13 13:57:20 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#define MSG_TO_SEND "Hello"
#define PACKET_SIZE 2048

#define PACKET_SIZE 2048

bool	verify_address(s_ping_data* ping_data)
{
	struct sockaddr_in;
	struct addrinfo hints;
	ft_memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int ret;
	ret = getaddrinfo(ping_data->args.address, NULL, &hints, &(ping_data->addrinfo));
	if (ret)
	{
		fprintf(stderr, "ft_ping: error: %s\n", gai_strerror(ret));
		return false;
	}
	for (struct addrinfo* T = ping_data->addrinfo; T != NULL; T = T->ai_next)
	{
		printf("ai_flags: %d\nai_family: %d\nai_socktype: %d\nai_protocol: %d\nai_addrlen %u\nai_cannonname: %s\n",
				T->ai_flags, T->ai_family, T->ai_socktype, T->ai_protocol, T->ai_addrlen, T->ai_canonname);
	}

	return true;
}

int		create_socket()
{
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	/*
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
		*/
	int opt_value = 1;
	if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &opt_value, sizeof(opt_value)) < 0)
		printf("setsockopt error : %s\n", strerror(errno));
		

	if (fd <= 0)
		printf("socket ret : %d %s\n", fd, strerror(errno));

	return fd;
}

unsigned short		create_checksum(void* header, int size)
{
	int checksum = 0;
	unsigned short* asptr = (unsigned short*) header;
	while (size > 1)
	{
		checksum += *asptr++;
		size -= 2;
	}
	if (size > 0)
		checksum += *(unsigned char*)asptr;
	while (checksum >> 16)
		checksum = (checksum & 0xFFFF) + (checksum >> 16);
	checksum = ~checksum; 
	return (unsigned short)checksum;
}

struct iphdr	create_ip_header(struct in_addr dst)
{
	struct iphdr hdr = {0};
	//struct in_addr src;	
	in_addr_t	src_t;
	in_addr_t	dst_t;
	(void)dst;

	hdr.version = 4; //ipv4
	hdr.ihl = 5; // header length, will always be 5 for ping
	hdr.tos = 0; // routing priority
	hdr.tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr) + ft_strlen(MSG_TO_SEND));
	hdr.id = 0; // id of the message, no use
	hdr.frag_off = 0; // can the msg be fragmented
	hdr.ttl = 64; // number of time packet can be redirected (needed to avoid infinite looping)
	hdr.protocol = 1;// stands for ICMP
	hdr.check = 0;
	int ret = inet_pton(AF_INET, "127.0.0.1", &src_t);
	if (ret == 0)
		printf("ret error on inet_pton on src:  %d", ret);
	ret = inet_pton(AF_INET, "127.0.0.1", &dst_t);
	if (ret == 0)
		printf("ret error on inet_pton on dst:  %d", ret);

	hdr.saddr = src_t;
	hdr.daddr = dst_t;
	//hdr.saddr = inet_addr("127.0.0.1");
	//hdr.daddr = inet_addr("127.0.0.1");
	//hdr.saddr = (in_addr_t)src.s_addr;
	//hdr.daddr = (in_addr_t)dst.s_addr;
	//hdr.check = create_checksum(&hdr, sizeof(struct iphdr));
	return hdr;
}

bool	send_packet(s_ping_data* ping_data)
{
	struct icmphdr icmp_header;
	unsigned char data[PACKET_SIZE];
	struct in_addr dst;

	if (inet_aton(ping_data->args.address, &dst) == 0)
	{
		printf("error with inet_aton\n");
		return false;
	}
	struct iphdr ip_hdr = create_ip_header(((struct sockaddr_in*)(ping_data->addrinfo->ai_addr))->sin_addr);

	ft_memset(&icmp_header, 0, sizeof(icmp_header));
	icmp_header.type = ICMP_ECHO;
	icmp_header.un.echo.id = 0;// TODO shouldn't be needed
	icmp_header.un.echo.sequence = 0;
	icmp_header.checksum = create_checksum(data + sizeof(ip_hdr), sizeof(icmp_header) + ft_strlen(MSG_TO_SEND));
	//icmp_header.checksum = 1;
	ft_memcpy(data, &ip_hdr, sizeof(ip_hdr));
	ft_memcpy(data + sizeof(ip_hdr), &icmp_header, sizeof(icmp_header));
	ft_memcpy(data + sizeof(ip_hdr) + sizeof(icmp_header), MSG_TO_SEND, ft_strlen(MSG_TO_SEND));

	int ret = sendto(ping_data->socketfd, data, sizeof(icmp_header) + ft_strlen(MSG_TO_SEND) * sizeof(char) + sizeof(ip_hdr),
			0, ping_data->addrinfo->ai_addr, sizeof(*(ping_data->addrinfo->ai_addr)));

	if (ret <= 0)
		printf("sendto ret : %d %s\n", ret, strerror(errno));
	else
		printf("sendto ret : %d \n", ret);
	struct msghdr msghdr;
	struct iovec iov;
	ft_memset(data, 0, sizeof(data));
	ft_memset(&msghdr, 0, sizeof(msghdr));
	msghdr.msg_iov = &iov;
	msghdr.msg_iovlen = 1;
	iov.iov_base = data;
	iov.iov_len = sizeof(data);
	/*
	socklen_t slen = 0;
	ret = recvfrom(ping_data->socketfd, data, sizeof(data), 0, NULL, &slen);
	if (ret <= 0)
		printf("recvmsg error ret : %d %s\n", ret, strerror(errno));
	else if (ret < (int)sizeof(icmp_header))
	{
		printf("recvmsg too small ret : %d%s\n", ret, strerror(errno));
		write(1, iov.iov_base, 64);
	}
	else {
		printf("recvmsg ok ret : %d \n", ret);
		memcpy(&icmp_header, data, sizeof(icmp_header));
		printf("Got ICMP packet with type: %d, id:0x%x, sequence 0x%x\n", icmp_header.type, icmp_header.un.echo.id, icmp_header.un.echo.sequence);
	}
	*/
	ret = recvmsg(ping_data->socketfd, &msghdr, 0);
	struct icmphdr* header_answer = (void*)data;

	printf("icmpheader result is equal to ICMP_REPLY : %d and is %hhd\n", header_answer->type == ICMP_ECHOREPLY, header_answer->type);
	printf("icmp data: id:0x%x, sequence = 0x%x\n",
			header_answer->un.echo.id,
			header_answer->un.echo.sequence);
	if (ret <= 0)
		printf("recvmsg ret : %d %s\n", ret, strerror(errno));
	else
	{
		printf("recvmsg ret : %d", ret);
		write(1, iov.iov_base, 64);
	}

	return true;
}
