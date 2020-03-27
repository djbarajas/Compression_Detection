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


void server_setup(int socket_type, int* sockfd,struct sockaddr_in* servaddr, struct sockaddr_in* peer_addr){
    
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
  
// Driver function 
int main() 
{ 
    int sockfd; 
    struct sockaddr_in servaddr, peer_addr;
    struct sockaddr recvd_sock;

    // setup buffer holder for storing file information
    char buff[1000]; 
    bzero(buff, sizeof(buff)); 

    //setup container for the recieved udp packets
    uint8_t* udp_payload;

    
    //setup and open connection for server (we are currently expecting TCP packets)
    server_setup(SOCK_STREAM,&sockfd,&servaddr,&peer_addr);
 

    // read the message that we obtained from the TCP connection (our file information)
    recv(sockfd, buff, sizeof(buff), 0); 

    
    // did we recieve the coreect information? Let's find out! 
    printf("From client: %s\n", buff); 

    // After chatting close the socket 
    close(sockfd); 

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

