#ifndef FT_PING_H
#define FT_PING_H

#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <math.h>

#define PACKET_SIZE 64

typedef struct s_rttTime
{
    double min;
    double max;
    double sum;
    double sum2;
}   t_rttTime;

int init_flag(int ac, char **av);
char *init_str(int ac, char **av);
int check_dns(struct addrinfo **adresse, struct addrinfo *hints, char *str);
void build_packet(char *packet, int seq);
int receive_packet(int sock, struct sockaddr_in *dest, int seq, char *param, t_rttTime *stats, int flags);
int creat_sock(char *param);
unsigned short checksum(void *b, int len);
void close_programme(char *str, int sock, struct addrinfo *adresse, char *param);
void ft_reverse_dns(struct sockaddr_in *dest, char *hostname); 
void    print_flag();

#endif