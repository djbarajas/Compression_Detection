/*
	THINGS TO DO 
	** FREE addrss_info after usage **
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include "read_json.h"

int main(int argc, char *argv[]){


	/**
	 * Pre-probing phase [TCP Connection and the recieving of packet information for compression detection]
	*/
	
	struct addrinfo init,* address_info;
	int socket_connect, addrinfo_retrieval, bind_success,listen_success, accept_success, recv_success;
	struct json tcp_info;

	memset(&init,0,sizeof(struct addrinfo));
	init.ai_family= AF_UNSPEC;
	init.ai_socktype=SOCK_STREAM;
	init.ai_flags = AI_PASSIVE;


	addrinfo_retrieval= getaddrinfo(NULL, argv[1], &init, &address_info);

	if (addrinfo_retrieval == -1 || address_info == NULL){
		perror("unable to retrieve client address information\n");
		exit(EXIT_FAILURE);
	}

	socket_connect = socket(address_info->ai_family,address_info->ai_socktype,address_info->ai_protocol);

	if (socket_connect == -1){
		perror("unable to initialize socket file descriptor\n");
		exit(EXIT_FAILURE);
	}


	bind_success = bind(socket_connect,address_info->ai_addr,address_info->ai_addrlen);

	if (bind_success ==-1){
		perror("unable to connect socket to address");
		exit(EXIT_FAILURE);
	}

	listen_success = listen(socket_connect,10);

	if (listen_success == -1){
		perror("error listening to incomming connections");
		exit(EXIT_FAILURE);
	}


	accept_success=accept(socket_connect,address_info->ai_addr,&address_info->ai_addrlen);
	if (accept_success == -1){
		perror("error accepting socket connections from client\n");
		exit(EXIT_FAILURE);
	}

	recv_success = recv(socket_connect,tcp_info,sizeof(tcp_info),0);

	if(recv_success == -1){
		perror("error recieving client information");
		exit(EXIT_FAILURE);
	}

	return 0;

}
