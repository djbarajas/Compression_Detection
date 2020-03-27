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
    //temp = strsep(&buff, " ");
    //temp = strsep(&buff, " ");
    tcp_info->server_ip = strsep(&buff, " ");
    //printf("%s\n", tcp_info->server_ip);
    tcp_info->src_prt_udp = strsep(&buff, " ");
    //printf("%s\n", tcp_info->src_prt_udp);
    tcp_info->dest_prt_udp = strsep(&buff, " ");
    //printf("%s\n", tcp_info->dest_prt_udp);
    tcp_info->dest_prt_tcp_head = strsep(&buff, " ");
    //printf("%s\n", tcp_info->dest_prt_tcp_head);
    tcp_info->dest_prt_tcp_tail = strsep(&buff, " ");
    //printf("%s\n", tcp_info->dest_prt_tcp_tail);
    tcp_info->prt_tcp = strsep(&buff, " ");
    //printf("%s\n", tcp_info->prt_tcp);
    tcp_info->payload_sz = atoi(strsep(&buff, " "));
    //printf("%d\n", tcp_info->payload_sz);
    tcp_info->in_time = atoi(strsep(&buff, " "));
    //printf("%d\n", tcp_info->in_time);
    tcp_info->num_of_packets = atoi(strsep(&buff, " "));
    //printf("%d\n", tcp_info->num_of_packets);
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
    //printf("%d\n", tcp_info->TTL);
    //printf("%s\n",temp);
}


// Function designed for chat between client and server. 
void func(int sockfd, char * buff, int len, struct json * tcp_info) 
{   
    char new[8] = "ERROR!!";
    char success[8] = "SUCCESS";

    bzero(buff, 1000); 

    // read the message from client and copy it in buffer 
    recv(sockfd, buff, 1000, 0); 
    // print buffer which contains the client contents 
    printf("From client: %s\n", buff); 
    int ret = tokenize(buff, tcp_info);
    if ( ret == 0 )
    {
        send(sockfd, new, 8, 0);

        close(sockfd);
        exit(1);
    }
    else
    {
        send(sockfd, success, 8, 0);
        return;
    }

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
    struct json tcp_info; 
    char buff[1000];
  
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
    func(connfd, buff, 1000, &tcp_info); 
        


    // After chatting close the socket 
    close(sockfd);

    //UDP STUFF
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
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
    servaddr.sin_port = htons(atoi(tcp_info.dest_prt_udp)); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  


    close(sockfd);
} 

