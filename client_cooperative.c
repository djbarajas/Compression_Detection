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
#include <arpa/inet.h>
#include "read_json.h"
#define SA struct sockaddr 


void func(int sockfd, char * buff, int len) 
{ 
    int n; 
    bzero(buff, len); 
    write(sockfd, buff, len); 
    //bzero(buff, sizeof(buff)); 
    //read(sockfd, buff, sizeof(buff)); 
} 
  
int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli;
    struct json tcp_info;

    char buff[1000] = {0};
    read_json(&tcp_info, "myconfig.json", buff); 
  	//printf("%s\n", buff);
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
 	while(*tcp_info.server_ip == ' ')
 	{
 		tcp_info.server_ip++;
 	}
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(tcp_info.server_ip);
    servaddr.sin_port = htons(atoi(tcp_info.prt_tcp)); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // function for chat 
    //func(sockfd, buff, 1000); 

  	//bzero(buff, strlen(buff)+1); 
    send(sockfd, buff, (strlen(buff)+1), 0);
    // close the socket 
    close(sockfd); 
} 
// int main(int argc, char *argv[]){

// 	/**
// 	 * Pre-probing phase [TCP Connection and packet information sharing]
// 	*/

// 	struct addrinfo init,* address_info;
// 	int socket_connect, addrinfo_retrieval, connect_success, send_success;
// 	struct json tcp_info;
// 	char total[1000] = {0};
	
// 	read_json(&tcp_info, "myconfig.json", total);
// 	printf("%s\n", total);

// 	memset(&init,0,sizeof(struct addrinfo));
// 	init.ai_family= AF_UNSPEC;
// 	init.ai_socktype=SOCK_STREAM;
// 	init.ai_flags = AI_PASSIVE;


// 	addrinfo_retrieval= getaddrinfo(tcp_info.server_ip,tcp_info.prt_tcp, &init, &address_info);

// 	if (addrinfo_retrieval == -1 || address_info == NULL){
// 		perror("unable to retrieve client address information\n");
// 		exit(EXIT_FAILURE);
// 	}


// 	socket_connect = socket(address_info->ai_family,address_info->ai_socktype,address_info->ai_protocol);
	

// 	if (socket_connect == -1){
// 		perror("unable to initialize socket file descriptor\n");
// 		exit(EXIT_FAILURE);
// 	}


// 	connect_success = connect(socket_connect,address_info->ai_addr,address_info->ai_addrlen);

// 	if (connect_success ==-1){
// 		perror("unable to connect socket to address\n");
// 		exit(EXIT_FAILURE);
// 	}

// 	// this is a temporary fill in
	

// 	send_success = send(socket_connect,&tcp_info,sizeof(struct json),0);


// 	if (send_success == -1){
// 		perror("unable to send message to the reviever\n");
// 		exit(EXIT_FAILURE);		
// 	}

// 	return 0;	

// }

