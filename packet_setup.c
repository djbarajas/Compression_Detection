#include "packet_setup.h"

/**
 * @file packet_setup.c
 *
 * source code exploited by our clients and server for setting up packet ids,
   reading packet payload, and initializing header and checksum
 *
 */


/**
 * id packet attachment function
 *
 * this function is responsible for generating a unique id for each packet
 *
 * @param bin | this is the binary version of the packet id (a binary strings)
 *
 * @param val | this is the unique id
 *
 * Does not envoke any local helper functions
 */
void 
packet_id_setup (char* bin, unsigned int val)
{
	unsigned int copy_of_value = val;
	for(int i = 15; i >= 0 ;i--)
	{
		bin[i] = (copy_of_value & 0b1) +'0';
		copy_of_value >>= 1;
	}
}

/**
 * high entropy payload setup function
 *
 * this function is responsible for generating and assigning data for a group of packets
 *
 * @param data | this is the group of packets we are sending out
 *
 * @param len | the amount of packets to fill in 
 *
 * Does not envoke any local helper functions
 */
void 
read_high_entropy_data(uint8_t * data, int len) 
{
	FILE* file_ptr = NULL;
	file_ptr =  fopen("/dev/random", "r");
	for (int i = 0; i < len-1; i++) {
		data[i] = getc(file_ptr);
	}
	fclose(file_ptr);
}


/**
 * IP checksum setup function
 *
 * this function is responsible for creating a unique checksum for our IP headers
 *
 * @param addr | this is the address of the IP header
 *
 * @param len | the length of the checksums
 *
 * Does not envoke any local helper functions
 */
uint16_t 
checksum(uint16_t *addr, int len)
{
    int count = len;
    register uint32_t sum = 0;
    uint16_t answer = 0;

    // Sum up 2-byte values until none or only one byte left.
    while(count > 1){
        sum += *(addr++);
        count -= 2;
    }

    // Add left-over byte, if any.
    if(count > 0) {
        sum += *(uint8_t *)addr;
    }

    // Fold 32-bit sum into 16 bits; we lose information by doing this,
    // increasing the chances of a collision.
    // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    // Checksum is one's compliment of sum.
    answer = ~sum;

    return (answer);
}


/**
 * TCP checksum setup function
 *
 * this function is responsible for creating a unique checksum for our TCP headers
 *
 * @param iphdr | this contains the IP header information
 *
 * @param tcphdr | this contains the TCP header information
 *
 * this function envokes checksum 
 * @see #checksum(uint16_t *addr, int len)
 *
 */
uint16_t 
tcp4_checksum (struct ip iphdr, struct tcphdr tcphdr)
{
    uint16_t svalue;
    char buf[IP_MAXPACKET], cvalue;
    char *ptr;
    int chksumlen = 0;

    // ptr points to beginning of buffer buf
    ptr = &buf[0];

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

    // Copy TCP length to buf (16 bits)
    svalue = htons (sizeof (tcphdr));
    memcpy (ptr, &svalue, sizeof (svalue));
    ptr += sizeof (svalue);
    chksumlen += sizeof (svalue);

    // Copy TCP source port to buf (16 bits)
    memcpy (ptr, &tcphdr.th_sport, sizeof (tcphdr.th_sport));
    ptr += sizeof (tcphdr.th_sport);
    chksumlen += sizeof (tcphdr.th_sport);

    // Copy TCP destination port to buf (16 bits)
    memcpy (ptr, &tcphdr.th_dport, sizeof (tcphdr.th_dport));
    ptr += sizeof (tcphdr.th_dport);
    chksumlen += sizeof (tcphdr.th_dport);

    // Copy sequence number to buf (32 bits)
    memcpy (ptr, &tcphdr.th_seq, sizeof (tcphdr.th_seq));
    ptr += sizeof (tcphdr.th_seq);
    chksumlen += sizeof (tcphdr.th_seq);

    // Copy acknowledgement number to buf (32 bits)
    memcpy (ptr, &tcphdr.th_ack, sizeof (tcphdr.th_ack));
    ptr += sizeof (tcphdr.th_ack);
    chksumlen += sizeof (tcphdr.th_ack);

    // Copy data offset to buf (4 bits) and
    // copy reserved bits to buf (4 bits)
    cvalue = (tcphdr.th_off << 4) + tcphdr.th_x2;
    memcpy (ptr, &cvalue, sizeof (cvalue));
    ptr += sizeof (cvalue);
    chksumlen += sizeof (cvalue);

    // Copy TCP flags to buf (8 bits)
    memcpy (ptr, &tcphdr.th_flags, sizeof (tcphdr.th_flags));
    ptr += sizeof (tcphdr.th_flags);
    chksumlen += sizeof (tcphdr.th_flags);

    // Copy TCP window size to buf (16 bits)
    memcpy (ptr, &tcphdr.th_win, sizeof (tcphdr.th_win));
    ptr += sizeof (tcphdr.th_win);
    chksumlen += sizeof (tcphdr.th_win);

    // Copy TCP checksum to buf (16 bits)
    // Zero, since we don't know it yet
    *ptr = 0; ptr++;
    *ptr = 0; ptr++;
    chksumlen += 2;

    // Copy urgent pointer to buf (16 bits)
    memcpy (ptr, &tcphdr.th_urp, sizeof (tcphdr.th_urp));
    ptr += sizeof (tcphdr.th_urp);
    chksumlen += sizeof (tcphdr.th_urp);

    return checksum ((uint16_t *) buf, chksumlen);
}


/**
 * UDP checksum setup function
 *
 * this function is responsible for creating a unique checksum for our UDP headers
 *
 * @param iphdr | this contains the IP header information
 *
 * @param udphdr | this contains the UDP header information
 *
 * @param payload | this is the body of the packet
 *
 * @param payloadlen | this is the length of the packet payload
 *
 * this function envokes checksum function
 * @see #checksum(uint16_t *addr, int len)
 *
 */
uint16_t 
udp4_checksum(struct ip iphdr, struct udphdr udphdr, uint8_t *payload, int payloadlen)
{
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

    return checksum((uint16_t *)buf,chksumlen);
}