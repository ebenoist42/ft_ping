/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 12:07:49 by ebenoist          #+#    #+#             */
/*   Updated: 2026/06/18 19:12:57 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

int running = 1;

void handler(int sig)
{
	(void)sig;
	running = 0;
}


int main (int ac, char **av)
{
    if (ac != 2)
    {
        write(2, "Error : Wrong number of arguments\n", 35);
        return(1);
    }
    int sock = creat_sock();
    struct addrinfo *adresse = NULL;
    struct addrinfo hints;
    if(check_dns(&adresse, &hints, av[1]))
        close_programme('\0', sock, adresse);
    struct sockaddr_in *dest = (struct sockaddr_in *)adresse->ai_addr;
    char reverse_dns[NI_MAXHOST];
    ft_reverse_dns(dest, reverse_dns);
    int seq = 1;
    char pack_icmp[PACKET_SIZE];
    signal(SIGINT, handler);
    while(running)
    {
        build_packet(pack_icmp, seq);
        ssize_t sent = sendto(sock, pack_icmp, PACKET_SIZE, 0,
                        (struct sockaddr *)dest, sizeof(*dest));
        if(sent < 0)
            close_programme("Error : sendto ", sock, adresse);
        receive_packet(sock, dest, seq, reverse_dns);
        seq++;
        sleep(1);
    }

    close_programme('\0', sock, adresse);
    return(0);
    
}
