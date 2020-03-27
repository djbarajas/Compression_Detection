/*CHECK IF WE SHOULD MEMSET THE VALUES BETWEEN THE TWO UDP TRAIN PACKETS*/

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



void server_setup(int socket_type, int* sockfd,struct sockaddr_in* servaddr, struct sockaddr_in* peer_addr, struct read_json){
    
    int connfd,len;

    // socket create and verification 
    *sockfd = socket(AF_INET, socket_type, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 

    bzero(servaddr, sizeof(*servaddr)); 
  
    // assign IP, PORT 
    servaddr->sin_family = AF_INET; 
    servaddr->sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr->sin_port = htons(8081); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(*sockfd, (SA*)servaddr, sizeof(*servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(*sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 

    len = sizeof(*peer_addr); 
  
    // Accept the data packet from client and verification 
    connfd = accept(*sockfd, (SA*)peer_addr, peer_addr); 

    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
}

int tokenize(char * buff, struct json * tcp_info )
{
    char * temp;
    tcp_info->server_ip = strsep(&buff, " ");
    tcp_info->src_prt_udp = strsep(&buff, " ");
    tcp_info->dest_prt_udp = strsep(&buff, " ");
    tcp_info->dest_prt_tcp_head = strsep(&buff, " ");
    tcp_info->dest_prt_tcp_tail = strsep(&buff, " ");
    tcp_info->prt_tcp = strsep(&buff, " ");
    tcp_info->payload_sz = atoi(strsep(&buff, " "));
    tcp_info->in_time = atoi(strsep(&buff, " "));
    tcp_info->num_of_packets = atoi(strsep(&buff, " "));
    tcp_info->TTL = atoi(strsep(&buff, " "));

    if (tcp_info->server_ip == NULL || tcp_info->src_prt_udp == NULL ||
        tcp_info->dest_prt_udp == NULL || tcp_info->dest_prt_tcp_head == NULL ||
        tcp_info->dest_prt_tcp_tail == NULL || tcp_info->prt_tcp == NULL ||
        tcp_info->prt_tcp == NULL || tcp_info->payload_sz < 1 ||
        tcp_info->in_time < 1 || tcp_info->num_of_packets < 1 ||
        tcp_info->TTL < 0)
    {
        printf("NOT ENOUGH INFO CLOSING SOCKET\n");
        return 0;
    }
    return 1;

}


  
// Driver function 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
    struct json tcp_info; 
    char buff[1000];
  

    //setup and open new connection for server (we are currently expecting UDP packets that divide to two parts)    
    server_setup(SOCK_DGRAM,&sockfd,&servaddr,&peer_addr);
    // we want to recieve the low entropy packet train (Quantity: 6000)    
    for (int i=0;i<6000;i++){        
        recvfrom(sockfd, udp_payload, sizeof(udp_payload),0, &recvd_sock,sizeof(recvd_sock));
    }
    // now we want to recieve the high entropy packet train (Quantity: 6000)    
    for (int i=0;i<6000;i++){        
        recvfrom(sockfd, udp_payload, sizeof(udp_payload),0, &recvd_sock,sizeof(recvd_sock));
    }

    close(sockfd);
} 

