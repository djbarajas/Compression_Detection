#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>


int main(int argc, char *argv[]){

	struct addrinfo init,* address_info;
	int socket_connect, addrinfo_retrieval;

	memset(&init,0,sizeof(struct addrinfo));

	/*
		first argument denotes the host we are trying to conect to (name/IP address)
		second argument denotes the selected port of our host
		third argument 
	*/
	addrinfo_retrieval= getaddrinfo(argv[0], argv[1], &init, &address_info);

	if (addrinfo_retrieval != 0){
		perror("unable to retrieve client address information\n");
		exit(EXIT_FAILURE);
	}

	socket_connect = socket(address_info->ai_family,address_info->ai_socktype,address_info->ai_protocol);

	if (socket_connect == 0){
		perror("unable to initialize socket file descriptor\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
