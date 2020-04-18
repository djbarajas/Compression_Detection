#ifndef _PKT_STP_H_
#define _PKT_STP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <netinet/tcp.h>      // struct tcphdr
#include <netinet/udp.h>      // struct tcphdr
#include <sys/ioctl.h> 
#include <netinet/ip.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <time.h>
#include <pcap.h>
#include <sys/wait.h>


void 
packet_id_setup (char* bin, unsigned int val);

void 
read_high_entropy_data(uint8_t * data, int len);

uint16_t
udp4_checksum(struct ip iphdr, struct udphdr udphdr, uint8_t *payload, int payloadlen);

uint16_t 
tcp4_checksum (struct ip iphdr, struct tcphdr tcphdr);

uint16_t 
checksum(uint16_t *addr, int len);

#endif