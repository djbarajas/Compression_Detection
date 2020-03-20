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


int main(int argc, char *argv[]){

	struct addrinfo init,* address_info;
	int socket_connect, addrinfo_retrieval, bind_success,listen_success, accept_success;

	memset(&init,0,sizeof(struct addrinfo));
	init.ai_family= AF_UNSPEC;
	init.ai_socktype=SOCK_STREAM;
	init.ai_flags = AI_PASSIVE;

	/*
		*Argument A: denotes the host we are trying to conect to (name/IP address)
		*Argument B: denotes the name of the service or port number we are listening on
		*Argument C: is our iinitial address information setup (restrictions)
		*Argument D: is a linked list of address information  (based on rstrictions)
		
	*/
	addrinfo_retrieval= getaddrinfo(NULL, argv[1], &init, &address_info);

	if (addrinfo_retrieval == -1){
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

	listen_success = listen(socket_connect,20);

	if (listen_success == -1){
		perror("error listening to incomming connections");
		exit(EXIT_FAILURE);
	}


	for (int i=0;i<6000;i++){
		accept_success=accept(socket_connect,address_info->ai_addr,&address_info->ai_addrlen);
		if (accept_success == -1){
			perror("error accpeting socket connections from client\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;

}




