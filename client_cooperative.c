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


void packet_setup(struct json packet_info, int socket_type, int* sockfd,struct sockaddr_in* servaddr){
    
    *sockfd = socket(AF_INET, socket_type, 0); 

    if (*sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n");

    bzero(servaddr, sizeof(*servaddr)); 

    while(*packet_info.server_ip == ' ')
    {
        packet_info.server_ip++;
    }

    servaddr->sin_family = AF_INET; 
    servaddr->sin_addr.s_addr = inet_addr(packet_info.server_ip);
    servaddr->sin_port = htons(atoi(packet_info.prt_tcp)); 

}

// Allocate memory for an array of unsigned chars.
uint8_t * allocate_ustrmem(int len)
{
  void *tmp;

  if (len <= 0) {
    fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_ustrmem().\n", len);
    exit (EXIT_FAILURE);
  }

  tmp = (uint8_t *) malloc (len * sizeof (uint8_t));
  if (tmp != NULL) {
    memset (tmp, 0, len * sizeof (uint8_t));
    return (tmp);
  } else {
    fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_ustrmem().\n");
    exit (EXIT_FAILURE);
  }
}

void func(int sockfd, char * buff, int len) 
{ 
    int n; 
    bzero(buff, len); 
    write(sockfd, buff, len); 
} 
  
int main() 
{ 
    /**
        Pre-probing phase [send in packet information through TCP connection] 
    */

    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli;
    struct json packet_info;
    uint8_t *data;
    

    char buff[1000] = {0};
    read_json(&packet_info, "myconfig.json", buff); 

    packet_setup(packet_info, SOCK_STREAM, &sockfd, &servaddr);


    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  

    send(sockfd, buff, (strlen(buff)+1), 0);


    close(sockfd); 


    /**
        Probing phase [send in UDP packet trains of high and low entropy data of quantity 6000] 
    */


    packet_setup(packet_info, SOCK_DGRAM, &sockfd,&servaddr);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 

    //let's setup UDP payload 

    data = allocate_ustrmem(16);

    for (int i=0;i<6000;i++){

        if(sendto(sockfd,data,sizeof(data),0,servaddr.sin_addr,servaddr.ai_addrlen)<=0){
            fprintf (stderr, "ERROR: unable to send UDP dataset to server.\n");
            exit (EXIT_FAILURE);
        } 
        else 
            printf("packet %d has been sent successfully\n",(i+1)); 
    }

    close(sockfd); 

}   









