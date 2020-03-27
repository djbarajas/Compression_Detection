#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h> 
#include <string.h>
#include <arpa/inet.h>
#include "read_json.h"
#define SA struct sockaddr 
#define IP_DONTFRAG 1


void read_high_entropy_data(uint8_t * data, int len){
    FILE* file_ptr = NULL;
    char temp;
    file_ptr =  fopen("/dev/random", "r");
    for (int i = 0; i < len; i++){
        temp = getc(file_ptr);
        int t = atoi(&temp);
        if (t > 1){
            t = 1;
        }
        data[i] = t;
        printf("%d\n", data[i]);
    }


}

void packet_setup(struct json packet_info, int socket_type, int* sockfd,struct sockaddr_in* clientaddr){
    
    *sockfd = socket(AF_INET, socket_type, 0); 

    if (*sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n");

    bzero(clientaddr, sizeof(*clientaddr)); 

    while(*packet_info.server_ip == ' ')
    {
        packet_info.server_ip++;
    }

    clientaddr->sin_family = AF_INET; 
    clientaddr->sin_addr.s_addr = inet_addr(packet_info.server_ip);
    clientaddr->sin_port = htons(atoi(packet_info.prt_tcp)); 

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

    int sockfd, connfd, val, clientlen; 
    struct sockaddr_in clientaddr, cli;
    struct json packet_info;
    uint8_t *data;
    

    char buff[1000] = {0};
    read_json(&packet_info, "myconfig.json", buff); 

    packet_setup(packet_info, SOCK_STREAM, &sockfd, &clientaddr);


    if (connect(sockfd, (SA*)&clientaddr, sizeof(clientaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  


    send(sockfd, buff, (strlen(buff)+1), 0);


    close(sockfd); 


    /**
        Probing phase [send in UDP packet trains of high and low entropy data each of quantity 6000] 
    */


    packet_setup(packet_info, SOCK_DGRAM, &sockfd,&clientaddr);

    val=IP_PMTUDISC_DO;
    if (setsockopt(sockfd, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val))<0){
        printf("unable to set DONT_FRAGMENT bit...\n"); 
        exit(0); 
    }
    else
        printf("DONT_FRAGMENT bit set successfully..\n");


    if (connect(sockfd, (SA*)&clientaddr, sizeof(clientaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 


    //let's setup UDP low entropy payload 

    data = allocate_ustrmem(packet_info.payload_sz);
    clientlen = sizeof(clientaddr);

    for (int i=0;i<packet_info.num_of_packets;i++){

        if(sendto(sockfd,data,sizeof(data),0,(struct sockaddr *) &clientaddr,clientlen)<=0){
            fprintf (stderr, "ERROR: unable to send UDP dataset to server.\n");
            exit (EXIT_FAILURE);
        } 
        else 
            printf("packet %d has been sent successfully\n",(i+1)); 
    }

    //let's setup UDP high entropy payload 
   read_high_entropy_data(data,packet_info.payload_sz);
    
    // wait Inter-Measurement Time (γ) seconds before sending the next set of UDP packets
    sleep(packet_info.in_time);
    
    for (int i=0;i<packet_info.num_of_packets;i++){

        if(sendto(sockfd,data,sizeof(data),0,(struct sockaddr *) &clientaddr,clientlen)<=0){
            fprintf (stderr, "ERROR: unable to send UDP dataset to server.\n");
            exit (EXIT_FAILURE);
        } 
        else 
            printf("packet %d has been sent successfully\n",(i+1)); 
    }
    close(sockfd); 

}   




} 
