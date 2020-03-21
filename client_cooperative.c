/*
	THINGS TO DO 
	** FREE addrss_info after usage **
	** check validity of addressinfo  == NULL **
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h> 
#include <string.h>
#include "read_json.h"


int main(int argc, char *argv[]){

	/**
	 * Pre-probing phase [TCP Connection and packet information sharing]
	*/

	struct addrinfo init,* address_info;
	int socket_connect, addrinfo_retrieval, connect_success, send_success;
	struct json tcp_info;
	
	read_json(&tcp_info, "myconfig.json");

	memset(&init,0,sizeof(struct addrinfo));
	init.ai_family= AF_UNSPEC;
	init.ai_socktype=SOCK_STREAM;
	init.ai_flags = AI_PASSIVE;


	addrinfo_retrieval= getaddrinfo(tcp_info.server_ip,tcp_info.prt_tcp, &init, &address_info);

	if (addrinfo_retrieval == -1 || address_info == NULL){
		perror("unable to retrieve client address information\n");
		exit(EXIT_FAILURE);
	}


	socket_connect = socket(address_info->ai_family,address_info->ai_socktype,address_info->ai_protocol);
	

	if (socket_connect == -1){
		perror("unable to initialize socket file descriptor\n");
		exit(EXIT_FAILURE);
	}


	connect_success = connect(socket_connect,address_info->ai_addr,address_info->ai_addrlen);

	if (connect_success ==-1){
		perror("unable to connect socket to address\n");
		exit(EXIT_FAILURE);
	}

	// this is a temporary fill in
	

	send_success = send(socket_connect,&tcp_info,sizeof(tcp_info),0);

	if (send_success == -1){
		perror("unable to send message to the reviever\n");
		exit(EXIT_FAILURE);		
	}

	return 0;	

}

