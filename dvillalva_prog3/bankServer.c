#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "banking.h"

#define NUM_ACCTS   100

//ip 	129.108.32.2
//port 	26207

typedef struct
{
    unsigned int 	balance;
    pthread_mutex_t mutex;
} sBANK_ACCT_DATA;

sBANK_ACCT_DATA acctData[NUM_ACCTS];

int setupTCPServer(unsigned int portNum, int maxConn)
{

//	int sockfd;
//	int newSocket;
//	struct sockaddr_in newAddr;
//	socklen_t addr_size;
//	char buffer[1024];

    int servSocket;
    struct sockaddr_in servAddr;

    /* Setup address */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(0);    /* Accept connections on an IP address */
    servAddr.sin_port = htons(portNum);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip address


    /* Create socket */
    if((servSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to create socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    printf("[+]Server Socket Created Successfully.\n");


    /* Bind socket for server */
    if(bind(servSocket,(struct sockaddr *) &servAddr,sizeof(servAddr)) < 0)
    {
        printf("Failed to bind socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

	printf("[+]Bind to Port number %d.\n", portNum);

    /* Setup socket for listening */
    if((listen(servSocket, maxConn)) < 0)
    {
        printf("Failed to listen on socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

	printf("[+]Listening...\n");

	int newSocket;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[1024];

	newSocket = accept(servSocket, (struct sockaddr*)&newAddr, &addr_size);

	strcpy(buffer, "Hello");
	send(newSocket, buffer, strlen(buffer), 0);
	printf("[+]Closing the connection.\n");

	close (newSocket);
    /* return the socket handle */
    return servSocket;

}

int main(int argc, char **argv)
{
	unsigned int portNum;

  	if(argc != 2)
    {
        printf("Usage: bankServer portNum\n");
        return -1;
    }



	/* Setup TCP port number */
	portNum = atoi(argv[1]);
	//portNum = strtol(argv[1],NULL, 10);

	setupTCPServer(portNum, 5);

//	while(1)
//	{
//		//banking options
//	}


//	newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
//
//	strcpy(buffer, "Hello");
//	send(newSocket, buffer, strlen(buffer), 0);
//	printf("[+]Closing the connection.\n");






	return 0;
}
