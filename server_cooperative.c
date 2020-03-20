/*
	THINGS TO DO 
	** FREE addrss_info after usage **
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char *argv[]){

	struct addrinfo init,* address_info;
	int socket_connect, addrinfo_retrieval;

	memset(&init,0,sizeof(struct addrinfo));
	init.ai_family= AF_UNSPEC;
	init.ai_socktype=SOCK_STREAM;
	init.ai_flags = AI_PASSIVE;

	/*
		*Argument A: denotes the host we are trying to conect to (name/IP address)
		*Argument B: denotes the name of the service or port number we are listening on
		*Argument C: is our initial address information setup
		*Argument D: is a linked list of address information returned (filled in by the function)
		
	*/
	addrinfo_retrieval= getaddrinfo(NULL, argv[0], &init, &address_info);

	if (addrinfo_retrieval != 0){
		perror("unable to retrieve client address information\n");
		exit(EXIT_FAILURE);
	}

	socket_connect = socket(address_info->ai_family,address_info->ai_socktype,address_info->ai_protocol);

	if (socket_connect != 0){
		perror("unable to initialize socket file descriptor\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

