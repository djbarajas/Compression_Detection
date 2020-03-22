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
#include <unistd.h>
#include <arpa/inet.h>
#include "read_json.h"
#define SA struct sockaddr

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
    char buff[1000]; 
    int n; 

    bzero(buff, sizeof(buff)); 

    // read the message from client and copy it in buffer 
    recv(sockfd, buff, sizeof(buff), 0); 
    // print buffer which contains the client contents 
    printf("From client: %s\n", buff); 
    //bzero(buff, 1000); 
    //n = 0; 

    // and send that buffer to client 
    //write(sockfd, buff, sizeof(buff)); 
 
} 
  
// Driver function 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(8081); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    // Function for chatting between client and server 
    func(connfd); 
  
    // After chatting close the socket 
    close(sockfd); 
} 

// int main(int argc, char *argv[]){


// 	/**
// 	 * Pre-probing phase [TCP Connection and retrieval of packet information for compression detection]
// 	*/
	
// 	struct addrinfo init,* address_info;
// 	int socket_connect, addrinfo_retrieval, bind_success,listen_success, accept_success, recv_success;
// 	struct json tcp_info;
// 	char info[1000];


// 	memset(&init,0,sizeof(struct addrinfo));
// 	init.ai_family= AF_UNSPEC;
// 	init.ai_socktype=SOCK_STREAM;
// 	init.ai_flags = AI_PASSIVE;


// 	addrinfo_retrieval= getaddrinfo(NULL, argv[1], &init, &address_info);

// 	if (addrinfo_retrieval == -1 || address_info == NULL){
// 		perror("unable to retrieve client address information\n");
// 		exit(EXIT_FAILURE);
// 	}

// 	socket_connect = socket(address_info->ai_family,address_info->ai_socktype,address_info->ai_protocol);

// 	if (socket_connect == -1){
// 		perror("unable to initialize socket file descriptor\n");
// 		exit(EXIT_FAILURE);
// 	}


// 	bind_success = bind(socket_connect,address_info->ai_addr,address_info->ai_addrlen);

// 	if (bind_success ==-1){
// 		perror("unable to connect socket to address");
// 		exit(EXIT_FAILURE);
// 	}

// 	listen_success = listen(socket_connect,10);

// 	if (listen_success == -1){
// 		perror("error listening to incomming connections");
// 		exit(EXIT_FAILURE);
// 	}


// 	accept_success=accept(socket_connect,address_info->ai_addr,&address_info->ai_addrlen);
// 	if (accept_success == -1){
// 		perror("error accepting socket connections from client\n");
// 		exit(EXIT_FAILURE);
// 	}

// 	recv_success = recv(accept_success,&tcp_info,sizeof(struct json),0);

// 	if(recv_success == -1){
// 		perror("error recieving client information");
// 		exit(EXIT_FAILURE);
// 	}

// 	//close(socket_connect);
// 	printf("%s\n",tcp_info.server_ip);
// 	return 0;

// }
