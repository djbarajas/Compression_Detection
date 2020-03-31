#include <stdio.h>
#include "read_json.h"

/*
	this class is responsible for compression detection with an uncooperative server 
	uncooperative denotes inability to onbtain control of the server (but its still responsive)
*/

// we have two different checksums: One for our UDP packets and the other for the TCP connection.

// Build IPv4 UDP pseudo-header and call checksum function.
uint16_t udp4_checksum (struct ip iphdr, struct udphdr udphdr, uint8_t *payload, int payloadlen){
  char buf[IP_MAXPACKET];
  char *ptr;
  int chksumlen = 0;
  int i;

  ptr = &buf[0];  // ptr points to beginning of buffer buf

  // Copy source IP address into buf (32 bits)
  memcpy (ptr, &iphdr.ip_src.s_addr, sizeof (iphdr.ip_src.s_addr));
  ptr += sizeof (iphdr.ip_src.s_addr);
  chksumlen += sizeof (iphdr.ip_src.s_addr);

  // Copy destination IP address into buf (32 bits)
  memcpy (ptr, &iphdr.ip_dst.s_addr, sizeof (iphdr.ip_dst.s_addr));
  ptr += sizeof (iphdr.ip_dst.s_addr);
  chksumlen += sizeof (iphdr.ip_dst.s_addr);

  // Copy zero field to buf (8 bits)
  *ptr = 0; ptr++;
  chksumlen += 1;

  // Copy transport layer protocol to buf (8 bits)
  memcpy (ptr, &iphdr.ip_p, sizeof (iphdr.ip_p));
  ptr += sizeof (iphdr.ip_p);
  chksumlen += sizeof (iphdr.ip_p);

  // Copy UDP length to buf (16 bits)
  memcpy (ptr, &udphdr.len, sizeof (udphdr.len));
  ptr += sizeof (udphdr.len);
  chksumlen += sizeof (udphdr.len);

  // Copy UDP source port to buf (16 bits)
  memcpy (ptr, &udphdr.source, sizeof (udphdr.source));
  ptr += sizeof (udphdr.source);
  chksumlen += sizeof (udphdr.source);

  // Copy UDP destination port to buf (16 bits)
  memcpy (ptr, &udphdr.dest, sizeof (udphdr.dest));
  ptr += sizeof (udphdr.dest);
  chksumlen += sizeof (udphdr.dest);

  // Copy UDP length again to buf (16 bits)
  memcpy (ptr, &udphdr.len, sizeof (udphdr.len));
  ptr += sizeof (udphdr.len);
  chksumlen += sizeof (udphdr.len);

  // Copy UDP checksum to buf (16 bits)
  // Zero, since we don't know it yet
  *ptr = 0; ptr++;
  *ptr = 0; ptr++;
  chksumlen += 2;

  // Copy payload to buf
  memcpy (ptr, payload, payloadlen);
  ptr += payloadlen;
  chksumlen += payloadlen;

  // Pad to the next 16-bit boundary
  for (i=0; i<payloadlen%2; i++, ptr++) {
    *ptr = 0;
    ptr++;
    chksumlen++;
  }

  return checksum ((uint16_t *) buf, chksumlen);
}


// Computing the internet checksum (RFC 1071).
// Note that the internet checksum does not preclude collisions.
uint16_t checksum (uint16_t *addr, int len){
  int count = len;
  register uint32_t sum = 0;
  uint16_t answer = 0;

  // Sum up 2-byte values until none or only one byte left.
  while (count > 1) {
    sum += *(addr++);
    count -= 2;
  }

  // Add left-over byte, if any.
  if (count > 0) {
    sum += *(uint8_t *) addr;
  }

  // Fold 32-bit sum into 16 bits; we lose information by doing this,
  // increasing the chances of a collision.
  // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
  while (sum >> 16) {
    sum = (sum & 0xffff)+(sum >> 16);
  }

  // Checksum is one's compliment of sum.
  answer = ~sum;

  return (answer);
}


int main(int argc, char **argv){

  /* 
      unlike our server_cooperative/client_cooperative  we are using raw sockets for deeper control over the
      packet data specifications (layers and payload)
  */

  if (argc != 2){
      fprintf (stderr, "ERROR: Too few or many arguments.\n");
      exit (EXIT_FAILURE);
  }

  //read JSON file to obtain data (TTL will be our main variable)
  struct json packet_info;
  char buff[1000] = {0};
  read_json(&packet_info, argv[1], buff);

  // prepare ip header for TCP manually
  int* ip_flags;
  struct ip ip_header;
  ip_header.ip_hl =IP4_HDRLEN/sizeof(uint32_t);
  ip_header.ip_v =4;
  ip_header.ip_tos = 0;
  ip_header.ip_len = htons(IP4_HDRLEN + TCP_HDRLEN + datalen); // htons makes sure we are using little-endian byte order
  ip_header.ip_id= htons(0);

  ip_flags[0] =0; // static unused bit
  ip_flags[1]=0; // DF flag
  ip_flags[2]=0; // MF flag
  ip_flags[3]=0; // fragment offset

  // position flag results into corresponding sectors in ip_off by shifting the bits to correct order 
  ip_header.ip_off = htons ((ip_flags[0] << 15) + (ip_flags[1] << 14) + (ip_flags[2] << 13) +  ip_flags[3]); 

  ip_header.ip_ttl =packet_info.TTL;
  iphdr.ip_p = IPPROTO_TCP;

  // start with a single head SYN packet (to port x) --> this will trigger RST packets to be sent from the server

  // follow it with a train of UDP packets (ignore ICMP packets sent back in this part)

  // end with a single tail SYN packet (to port y) --> this will trigger RST packets to be sent from the server

  //calculate the difference between arrival time of the two RST packets for compression analysis (loss may occur)
	
	
  return 0;
}
