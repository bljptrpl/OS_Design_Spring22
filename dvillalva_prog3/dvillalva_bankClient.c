/*
 * server.c
 *
 *  Created on: Mar 21, 2022
 *      Author: daniel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "dvillalva_banking.h"

#define NUM_ACCTS   100

//ip 	129.108.32.2
//port 	26207

int setupTCPClient(char *servIPAddr, unsigned int portNum)
{
    int clientSocket;
    struct sockaddr_in servAddr;

    /* Setup address of server */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIPAddr);
    servAddr.sin_port = htons(portNum);

    /* Create socket */
    if((clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to create socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Connect socket to server */
    int w = connect(clientSocket,(struct sockaddr *) &servAddr,sizeof(servAddr));
    if(w < 0)
    {
        printf("Failed to connect socket to %s:%d; Error %d: %s\n", servIPAddr, portNum, errno, strerror(errno));
        return -1;
    }

    return clientSocket;
}
int convertCommand(char *command)
{
	int c;
	if(command == 'B')
		c = BANK_TRANS_INQUIRY;
	else if(command == 'W')
		c = BANK_TRANS_WITHDRAW;
	else if(command == 'D')
		c = BANK_TRANS_DEPOSIT;
	else
		printf("error converting command\n");
	return c;
}

int main(int argc, char **argv)
{
	char serverIP[15];
	unsigned int portNum;
	sBANK_PROTOCOL client, server;
	char *command = malloc(sizeof(char));

	if(argc != 6)
	{
		printf("Usage: bankClient servIPAddr servPortNum command acctNum value\n");
		return -1;
	}

	/* Setup */
	strcpy(serverIP, argv[1]);
	portNum = atoi(argv[2]);
	strcpy(command, argv[3]);
	client.trans = convertCommand(*command);
	client.acctnum = atoi(argv[4]);
	client.value = atoi(argv[5]);

	printf("servIPAddr:%s \tservPortNum:%d \tcommand:%d \tacctNum:%d \tvalue:%d\n\n", serverIP, portNum, client.trans, client.acctnum, client.value);

	/* Create socket */
	int sock;
	if((sock = setupTCPClient(serverIP, portNum)) < 0)
    {
		return -1;
    }
	printf("Socket created & connected! Nice!\n\n");

	/* Send structure to Server*/
	printf("Client: Account number: %d\tValue: %d\tTransaction: %d\n", client.acctnum, client.value, client.trans);
	send(sock, &client, sizeof(client),0);


	/* Receive structure from Server */
	recv(sock, &server, sizeof(server),0);
	printf("Server: Account number: %d\tValue: %d\tTransaction: %d\n", server.acctnum, server.value, server.trans);

	/* Close socket */
	close(sock);
	printf("\nDisconnected from Server.\n");


	return 0;
}
