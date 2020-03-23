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
  
    send(sockfd, buff, (strlen(buff)+1), 0);

    // close the socket 
    close(sockfd); 
} 
