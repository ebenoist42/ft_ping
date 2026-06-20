/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:11:35 by ebenoist          #+#    #+#             */
/*   Updated: 2026/06/18 19:18:42 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

int creat_sock()
{
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sock < 0)
    {
        perror("Error : socket\n");
        exit(1);
    }
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) // cree une regle de temp mac d'attente pour ce socket, until dans recvfrom, attente max 1s
    {
        perror("Error : setsockopt");
        close(sock);
        exit(1);
    }
    return(sock);
}

void build_packet(char *packet, int seq)
{
    struct icmphdr *icmp_hdr = (struct icmphdr *)packet;

    memset(packet, 0, PACKET_SIZE);
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.id = htons(getpid() & 0xFFFF); 
    icmp_hdr->un.echo.sequence = htons(seq);
    icmp_hdr->checksum = 0;
    struct timeval *tv = (struct timeval *)(packet + sizeof(struct icmphdr));
    gettimeofday(tv, NULL);
    icmp_hdr->checksum = checksum(packet, PACKET_SIZE);
}

void receive_packet(int sock, struct sockaddr_in *dest, int seq, char *reverse_dns)
{
    char            buffer[1024];
    ssize_t         received;
    struct ip       *ip_hdr;
    struct icmphdr  *icmp;
    struct timeval  *sent_time;
    struct timeval  now;
    int             ip_len;
    double          rtt;

    received = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
    if (received < 0)
    {
        printf("Request timeout for icmp_seq %d\n", seq);
        return;
    }
    ip_hdr = (struct ip *)buffer;
    ip_len = ip_hdr->ip_hl * 4;                      // taille du header IP à sauter
    icmp = (struct icmphdr *)(buffer + ip_len);       // début du header ICMP

    if (icmp->type == ICMP_ECHOREPLY
        && ntohs(icmp->un.echo.id) == (getpid() & 0xFFFF))
    {
        sent_time = (struct timeval *)(buffer + ip_len + sizeof(struct icmphdr));
        gettimeofday(&now, NULL);
        rtt = (now.tv_sec - sent_time->tv_sec) * 1000.0
            + (now.tv_usec - sent_time->tv_usec) / 1000.0;
        printf("%zd bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1f ms\n",
           received - ip_len, reverse_dns, inet_ntoa(dest->sin_addr),
           ntohs(icmp->un.echo.sequence), ip_hdr->ip_ttl, rtt);
    }
    else if (icmp->type == ICMP_TIME_EXCEEDED)
        printf("From %s icmp_seq=%d Time to live exceeded\n",
               inet_ntoa(dest->sin_addr), seq);
}