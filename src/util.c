/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 18:55:16 by ebenoist          #+#    #+#             */
/*   Updated: 2026/06/21 21:08:46 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"


void close_programme(char *str, int sock, struct addrinfo *adresse, char *param)
{
    if(str)
        perror(str);
    close(sock);
    if (adresse)
        freeaddrinfo(adresse);
    if(param)
        free(param);
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
        printf("Error : name invalide\n");
        return(1);
    }
    return(0);
}

void    print_flag()
{
    printf(
        "Usage: ping [OPTION...] HOST ...\n"
        "Send ICMP ECHO_REQUEST packets to network hosts.\n"
        " Options controlling ICMP request types:\n"
        "      --address              send ICMP_ADDRESS packets (root only)\n"
        "      --echo                 send ICMP_ECHO packets (default)\n"
        "      --mask                 same as --address\n"
        "      --timestamp            send ICMP_TIMESTAMP packets\n"
        "  -t, --type=TYPE            send TYPE packets\n\n"
        " Options valid for all request types:\n\n"
        "  -c, --count=NUMBER         stop after sending NUMBER packets\n"
        "  -d, --debug                set the SO_DEBUG option\n"
        "  -i, --interval=NUMBER      wait NUMBER seconds between sending each packet\n"
        "  -n, --numeric              do not resolve host addresses\n"
        "  -r, --ignore-routing       send directly to a host on an attached network\n"
        "      --ttl=N                specify N as time-to-live\n"
        "  -T, --tos=NUM              set type of service (TOS) to NUM\n"
        "  -v, --verbose              verbose output\n"
        "  -w, --timeout=N            stop after N seconds\n"
        "  -W, --linger=N             number of seconds to wait for response\n\n"
        " Options valid for --echo requests:\n\n"
        "  -f, --flood                flood ping (root only)\n"
        "      --ip-timestamp=FLAG    IP timestamp of type FLAG, which is one of\n"
        "                             \"tsonly\" and \"tsaddr\"\n"
        "  -l, --preload=NUMBER       send NUMBER packets as fast as possible before\n"
        "                             falling into normal mode of behavior (root only)\n"
        "  -p, --pattern=PATTERN      fill ICMP packet with given pattern (hex)\n"
        "  -q, --quiet                quiet output\n"
        "  -R, --route                record route\n"
        "  -s, --size=NUMBER          send NUMBER data octets\n\n"
        "  -?, --help                 give this help list\n"
        "      --usage                give a short usage message\n"
        "  -V, --version              print program version\n\n"
        "Mandatory or optional arguments to long options are also mandatory or optional\n"
        "for any corresponding short options.\n\n"
        "Options marked with (root only) are available only to superuser.\n\n"
        "Report bugs to <bug-inetutils@gnu.org>.\n"
    );
}