#ifndef _RD_JN_
#define _RD_JN_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct json
{
	char * server_ip;
	char * src_prt_udp;
	char * dest_prt_udp;
	char * dest_prt_tcp_head;
	char * dest_prt_tcp_tail;
	char * prt_tcp;
	int payload_sz;
	int in_time;
	int num_of_packets;
	int TTL;
};

void read_json (struct json * to_read, char * file_name);


#endif