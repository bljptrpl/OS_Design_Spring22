/*
 * server.c
 *
 *  Created on: Mar 21, 2022
 *      Author: daniel
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "dvillalva_banking.h"

#define NUM_ACCTS   100

//ip 	129.108.32.2
//port 	26207

typedef struct
{
    unsigned int 	balance;
    pthread_mutex_t mutex;
} sBANK_ACCT_DATA;

sBANK_ACCT_DATA acctData[NUM_ACCTS];


int main(int argc, char **argv)
{
	unsigned int portNum = 4444;
	int server_Socket, client_Socket;
	struct sockaddr_in servAddr;
	socklen_t addr_size;
	char buffer[1024];
	int n;

	/* Create socket */
	server_Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_Socket < 0)
	{
		printf("[-]Socket; Error %d: %s\n", errno, strerror(errno));
		return -1;
	}

	printf("[+]Server Socket Created.\n");

	/* Setup address */
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(0);    /* Accept connections on an IP address */
	servAddr.sin_port = htons(portNum);
	//servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip address

	n = bind(server_Socket,(struct sockaddr *) &servAddr,sizeof(servAddr));
    /* Bind socket for server */
	if(n < 0)
	{
        printf("[-] Bind; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

	printf("[+]Bind to Port number %d.\n", portNum);

	int maxConn = 5;
	/* Setup socket for listening */
	if((listen(server_Socket, maxConn)) < 0)
	{
		printf("Failed to listen on socket; Error %d: %s\n", errno, strerror(errno));
	    return -1;
	}

	printf("[+]Listening...\n");

	int newSocket;
	struct sockaddr_in newAddr;
	socklen_t newAddrLen = sizeof(newAddr);
	sBANK_PROTOCOL client;
	int data[1024];


while(1)
{
	//Accept
	newSocket = accept(server_Socket, (struct sockaddr*)&newAddr, &newAddrLen);
	printf("[+]Client connected.\n");

	//Receive
	recv(newSocket, &client, sizeof(client),0);
	printf("Client: Account number: %d\tValue: %d\tTransaction: %d\tBalance: %d\n", client.acctnum, client.value, client.trans,acctData[client.acctnum].balance);
	//semaphore lock
	pthread_mutex_init(&acctData[client.acctnum].mutex,NULL);
	if(pthread_mutex_lock(&acctData[client.acctnum].mutex) != 0)
		printf("mutex error!\n");
/************************************* START Critical Section ********************************/
	//transaction
	if(client.trans == 0) //Deposit
	{
		acctData[client.acctnum].balance =+ client.value;
	}
	else if(client.trans == 1) //Withdrawal
	{
		acctData[client.acctnum].balance =- client.value;

		if(acctData[client.acctnum].balance > 0)
		{
			acctData[client.acctnum].balance = 0;
			client.value = 0;
		}
	}
	else if(client.trans ==  2) //Inquiry
	{
		client.value = acctData[client.acctnum].balance;
	}
	else
	{
		printf("transaction error!\n");
	}


/************************************* END Critical Section ********************************/

	//semaphore unlock
	if(pthread_mutex_unlock(&acctData[client.acctnum].mutex) != 0)
		printf("mutex error!\n");
	pthread_mutex_destroy(&acctData[client.acctnum].mutex);

	//Send
	printf("Client: Account number: %d\tValue: %d\tTransaction: %d\tBalance: %d\n", client.acctnum, client.value, client.trans,acctData[client.acctnum].balance);
	send(newSocket, &client,sizeof(client), 0);
	close (newSocket);

}//while
	return 0;
}
