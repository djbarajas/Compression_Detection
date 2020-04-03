#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "read_json.h"
#define SA struct sockaddr
#define THRESH 100

int tokenize(char * buff, struct json * tcp_info )
{
    char * temp;
    while (*buff == ' ')
    {
        buff++;
    }

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
}

int server_setup(int socket_type, int* sockfd,struct sockaddr_in* servaddr, struct sockaddr_in* peer_addr){
    
    int connfd,len;

    // socket create and verification 
    *sockfd = socket(AF_INET, socket_type, 0); 
    if (*sockfd == -1) { 
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
    connfd = accept(*sockfd, (SA*)peer_addr, &len); 

    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    return connfd;
}


void recv_udp_train(struct json* tcp_info )
{
    int sockfd; 
    char buffer[1024]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY;
    printf("dest prt %d\n", atoi(tcp_info->dest_prt_udp) );
    servaddr.sin_port = htons(atoi(tcp_info->dest_prt_udp)); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/resuslt 
  
    n = recvfrom(sockfd, (char *)buffer, 1024,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 
    sendto(sockfd, (const char *)hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 
    printf("Hello message sent.\n"); 
}

  

int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
    struct json tcp_info; 

    char buff[1000];
  
    connfd = server_setup(SOCK_STREAM,&sockfd,&servaddr,&cli);
    func(connfd, buff, 1000, &tcp_info);
    printf("Ending connection\n");
    close(sockfd);

    sleep(20);

    char *buffer =  calloc(tcp_info.payload_sz, sizeof(char));  
    struct sockaddr_in  cliaddr;
    int fd; 
      
    // Creating socket file descriptor 
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 

      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(atoi(tcp_info.dest_prt_udp)); 
      
    // Bind the socket with the server address 
    if ( bind(fd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    else
    {
        printf("BINDED\n");
    }
      
    int  n; 
    clock_t start_t, end_t;
    double total_t, low_entropy, high_entropy;
  
    len = sizeof(cliaddr);  //len is value/resuslt 

    start_t = clock();
    for (int i=0;i<tcp_info.num_of_packets;i++){ 
      
        n = recvfrom(fd, (char *)buffer, tcp_info.payload_sz,  
               MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
               &len); 

    }
    end_t = clock();
    total_t = (((double)end_t) - ((double)start_t)) / ((double)CLOCKS_PER_SEC);
    low_entropy = total_t * 1000;
    printf("time : %f\n", low_entropy);

    sleep(tcp_info.in_time);

    start_t = clock();
    for (int i=0;i<tcp_info.num_of_packets;i++){        
        n = recvfrom(fd, (char *)buffer, tcp_info.payload_sz,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len);
    }
    end_t = clock();
    total_t = (((double)end_t) - ((double)start_t)) / ((double)CLOCKS_PER_SEC);
    high_entropy = total_t * 1000;
    printf("time 2 : %f\n", high_entropy);    


    close(fd);

    sleep(32);

    //final tcp connection

    struct sockaddr_in serv_end, cliaddr_end;
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&serv_end, sizeof(serv_end)); 
  
    // assign IP, PORT 
    serv_end.sin_family = AF_INET; 
    serv_end.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_end.sin_port = htons(8082); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&serv_end, sizeof(serv_end))) != 0) { 
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
    len = sizeof(cliaddr_end); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cliaddr_end, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 

    char compression[50];

    // now we will do the check: If (∆tH − ∆tL) is bigger than our threshold (100 ms) then we have compression
    if ((high_entropy - low_entropy) > THRESH)
    {
        strcpy(compression, "COMPRESSION DETECTED");
    }
    else
    {
        strcpy(compression, "COMPRESSION NOT DETECTED");

    }
    

    bzero(buff, sizeof(buff));


    // read the message from client and copy it in buffer 
    recv(sockfd, buff, 1000, 0);

    send(connfd, compression, 50, 0);



    free(buffer);
    close(sockfd);



} 

