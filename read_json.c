
#include "read_json.h"

void set_struct(struct json * to_read, char * line)
{
	if (strstr(line,"Server IP") != NULL)
	{
		to_read->server_ip = strdup(line);
		to_read->server_ip = to_read->server_ip + strcspn(to_read->server_ip, ":") + 3;
		*(to_read->server_ip + strcspn(to_read->server_ip, "\"")) = '\0';
	}
	else if (strstr(line,"Source Port Number UDP") != NULL)
	{
		to_read->src_prt_udp = strdup(line);
		to_read->src_prt_udp = to_read->src_prt_udp + strcspn(to_read->src_prt_udp, ":") + 3;
		*(to_read->src_prt_udp + strcspn(to_read->src_prt_udp, "\"")) = '\0';
	}
	else if (strstr(line,"Destination Port Number UDP") != NULL)
	{
		to_read->dest_prt_udp = strdup(line);
		to_read->dest_prt_udp = to_read->dest_prt_udp + strcspn(to_read->dest_prt_udp, ":") + 3;
		*(to_read->dest_prt_udp + strcspn(to_read->dest_prt_udp, "\"")) = '\0';
	}
	else if (strstr(line,"Destination Port Number TCP Head") != NULL)
	{
		to_read->dest_prt_tcp_head = strdup(line);
		to_read->dest_prt_tcp_head = to_read->dest_prt_tcp_head + strcspn(to_read->dest_prt_tcp_head, ":") + 3;
		*(to_read->dest_prt_tcp_head + strcspn(to_read->dest_prt_tcp_head, "\"")) = '\0';
	}
	else if (strstr(line,"Destination Port Number TCP Tail") != NULL)
	{
		to_read->dest_prt_tcp_tail = strdup(line);
		to_read->dest_prt_tcp_tail = to_read->dest_prt_tcp_tail + strcspn(to_read->dest_prt_tcp_tail, ":") + 3;
		*(to_read->dest_prt_tcp_tail + strcspn(to_read->dest_prt_tcp_tail, "\"")) = '\0';
	}
	else if (strstr(line,"Port Number for TCP") != NULL)
	{
		to_read->prt_tcp = strdup(line);
		to_read->prt_tcp = to_read->prt_tcp + strcspn(to_read->prt_tcp, ":") + 3;
		*(to_read->prt_tcp + strcspn(to_read->prt_tcp, "\"")) = '\0';
	}
	else if (strstr(line,"Payload Size") != NULL)
	{
		char *temp = strdup(line);
		temp = temp + strcspn(temp, ":") + 3;
		*(temp + strcspn(temp, "\"")) = '\0';
		to_read->payload_sz = atoi(temp);
	}
	else if (strstr(line,"Inter-Measurement Time") != NULL)
	{
		char *temp = strdup(line);
		temp = temp + strcspn(temp, ":") + 3;
		*(temp + strcspn(temp, "\"")) = '\0';
		to_read->in_time = atoi(temp);
	}
	else if (strstr(line,"Number of UDP Packets") != NULL)
	{
		char *temp = strdup(line);
		temp = temp + strcspn(temp, ":") + 3;
		*(temp + strcspn(temp, "\"")) = '\0';
		to_read->num_of_packets = atoi(temp);	
	}
	else if (strstr(line,"TTL") != NULL)
	{
		char *temp = strdup(line);
		temp = temp + strcspn(temp, ":") + 3;
		*(temp + strcspn(temp, "\"")) = '\0';
		to_read->TTL = atoi(temp);
	}
}

void read_json (struct json * to_read, char * file_name)
{
	FILE* file_ptr = NULL;
	file_ptr =  fopen(file_name, "r");
	char *buffer;
    size_t bufsize = 100;
    size_t characters;
    while (true)
    {
	    buffer = (char *) malloc(bufsize * sizeof(char));
	    if (buffer == NULL)
	    {
	    	continue;
	    }
	    characters = getline(&buffer,&bufsize,file_ptr);
    	if (feof(file_ptr))
    	{
    		free(buffer);
    		break;
    	}
    	set_struct(to_read, buffer);
    	free(buffer);
    }

    fclose(file_ptr);
}

int main(int argc, char ** argv)
{
	struct json to_read;
	read_json(&to_read, argv[1]);
	printf("%s\n", to_read.server_ip);
	printf("%s\n", to_read.src_prt_udp);
	printf("%s\n", to_read.dest_prt_udp);
	printf("%s\n", to_read.dest_prt_tcp_head);
	printf("%s\n", to_read.dest_prt_tcp_tail);
	printf("%s\n", to_read.prt_tcp);
	printf("%d\n", to_read.payload_sz);
	printf("%d\n", to_read.in_time);
	printf("%d\n", to_read.num_of_packets);
	printf("%d\n", to_read.TTL);
}