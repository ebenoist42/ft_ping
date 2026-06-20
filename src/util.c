/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 18:55:16 by ebenoist          #+#    #+#             */
/*   Updated: 2026/06/18 19:05:32 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"


void close_programme(char *str, int sock, struct addrinfo *adresse)
{
    if(str)
        perror(str);
    close(sock);
    if (adresse)
        freeaddrinfo(adresse);
    exit(1);
}

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;
    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int     check_dns(struct addrinfo **adresse, struct addrinfo *hints, char *str)
{
    memset(hints, 0, sizeof(*hints));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_RAW;
    hints->ai_protocol = IPPROTO_ICMP;
    int Dns_ok = getaddrinfo(str, NULL, hints, adresse);
    if(Dns_ok != 0)
    {
        printf("Error : name  invalide\n");
        return(1);
    }
    printf("Ip found\n");
    return(0);
}

void ft_reverse_dns(struct sockaddr_in *dest, char *hostname)
{
    if (getnameinfo((struct sockaddr *)dest, sizeof(*dest),
                    hostname, NI_MAXHOST, NULL, 0, 0) != 0)
        strcpy(hostname, inet_ntoa(dest->sin_addr));  // fallback : IP brute si pas de reverse
}