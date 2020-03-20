#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(void){

	int socket_connect;

	/* 
		here we used AF_INTET to indicate we are using IPv4, SOCK_STREAM for TCP connection, and 
	 	0 for protocol to indicate we are using IP
	 	Note: this initializes our internet socket
	 */
	socket_connect = socket(AF_INET,SOCK_STREAM,0);

	if (socket_connect == 0){
		perror("unable to initialize socket file descriptor\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
