/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 12:07:49 by ebenoist          #+#    #+#             */
/*   Updated: 2026/06/20 15:20:22 by ebenoist         ###   ########.fr       */
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
    t_rttTime stats = {0};
    stats.min = 9999999999.0;
    int sock = creat_sock();
    struct addrinfo *adresse = NULL;
    struct addrinfo hints;
    if(check_dns(&adresse, &hints, av[1]))
        close_programme('\0', sock, adresse);
    struct sockaddr_in *dest = (struct sockaddr_in *)adresse->ai_addr;
    int seq = 0;
    int received = 0;
    char pack_icmp[PACKET_SIZE];
    signal(SIGINT, handler);
    int send = 0;
    while(running)
    {
        build_packet(pack_icmp, seq);
        ssize_t sent = sendto(sock, pack_icmp, PACKET_SIZE, 0,
                        (struct sockaddr *)dest, sizeof(*dest));
        send++;
        if(sent < 0)
            close_programme("Error : sendto ", sock, adresse);
        if (receive_packet(sock, dest, seq, av[1], &stats))
            received++;
        seq++;
        sleep(1);
    }
    double avg = stats.sum / received;
    double stddev = sqrt(stats.sum2 / received - avg * avg);
    printf("--- %s ping statistics ---\n", av[1]);
    printf("%d packets transmitted, %d packets received, %d%% packet loss\n", send, received, ((send - received) * 100 / send));
    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", stats.min, avg, stats.max, stddev);
    close_programme('\0', sock, adresse);
    return(0);
    
}
