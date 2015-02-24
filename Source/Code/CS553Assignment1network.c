//============================================================================
// Name        : CS553Assignment1Network.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================


#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define TIMES 11
#define ITRN  10000000;
#define MAXTHREADS 16

struct arg
{
	long blockSize;
	int threadNo;
	int loopCnt;
	long itrn;
	int port;
	char *host;
	int n;
	int newSocketFd,socketFd;
	socklen_t clientLen;
	struct sockaddr_in clientAddr;
};

void error(char *msg)
{
	perror(msg);
	exit(1);
}

long GetTimeSpent(struct timeval end,struct timeval start)
{
	return (end.tv_sec - start.tv_sec)*1000000 + ((int)end.tv_usec - (int)start.tv_usec);
}



void NetworkTCPClient_Benchmark(long blockSize,long itrn, int port, int threadCnt, char *hostName)
{
	int i,count=0;
	int socketFd,n;
	struct sockaddr_in serverAddr;
	struct hostent *server;										// Defines host computer
	char buffer[128000];
	socketFd = socket (AF_INET,SOCK_STREAM,0);
	if(socketFd<0)
		error("error opening socket");
	server=gethostbyname(hostName);
	if(server==NULL)
		error("no such host");
	bzero((char *)&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	bcopy((char *)server->h_addr,(char *)&serverAddr.sin_addr.s_addr,server->h_length); 		// copies s_addr to h_addr, ie server address to host
	serverAddr.sin_port = htons(port);
	if(connect(socketFd,(struct sockaddr *) &serverAddr,sizeof(serverAddr))<0)
		error("cannot connect to server");
	bzero(buffer,blockSize);
	for(i =0;i<blockSize;i++)
		buffer[i]=i%10;
	n = write(socketFd,buffer,blockSize);
	if(n<0)
		error("error sending data to server");
	bzero(buffer,blockSize);
	n=read(socketFd,buffer,blockSize-1);
	if(n<0)
		error("error reading from server");
	count++;
	printf("%s\n",buffer);
}

long NetworkTCPClient(long blockSize,long itrn,int port, int threadCnt, char *hostName)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	NetworkTCPClient_Benchmark(blockSize,itrn,port,threadCnt,hostName);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}


void NetworkUDPClient_Benchmark(long blockSize,long itrn, int port, int threadCnt, char *hostName)
{
	int i;
	int socketFd,n;
	struct sockaddr_in serverAddr,from;
	struct hostent *server;										// Defines host computer
	char buffer[128000];
	int length;
	socketFd = socket (AF_INET,SOCK_DGRAM,0);
	if(socketFd<0)
		error("error opening socket");
	server=gethostbyname(hostName);
	if(server==NULL)
		error("no such host");
	bzero((char *)&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	bcopy((char *)server->h_addr,(char *)&serverAddr.sin_addr.s_addr,server->h_length); 		// copies s_addr to h_addr, ie server address to host
	serverAddr.sin_port = htons(port);
	if(connect(socketFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)
		error("cannot connect to server");
	bzero(buffer,blockSize);
	for(i =0;i<blockSize;i++)
		buffer[i]=i%10;
	length = sizeof(struct sockaddr_in);
	n=sendto(socketFd,buffer,blockSize,0,(const struct sockaddr *)&serverAddr,length);
	if(n<0)
		error("error sending data to server");
	bzero(buffer,blockSize);
	n = recvfrom(socketFd,buffer,20,0,(struct sockaddr *)&from,&length);
	if(n<0)
		error("error reading from server");
	printf("%s\n",buffer);
}

long NetworkUDPClient(long blockSize,long itrn,int port, int threadCnt, char *hostName)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	NetworkUDPClient_Benchmark(blockSize,itrn,port,threadCnt,hostName);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}
double LatencyInms(long result)
{
	return ((double)result)/1000;
}

double ThroughputInMBps(long result,long blockSize,long itrn)
{
	return ((double)(blockSize*itrn)/(1000000))/(((double)result/1000000));
}

void * TCPServerThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	char buffer[128000];
	while(1)
{	
	bzero(buffer,arguments->blockSize);
	arguments->n=read(arguments->newSocketFd,buffer,arguments->blockSize-1);
	if(arguments->n<0)
		error("error reading from socket");
	arguments->n=write(arguments->newSocketFd,"message received",16);
	if(arguments->n<0)
		error("error writing to socket");
	}
}

void NetworkTCPServer(long blockSize, long itrn, int port, int threadCnt)
{
	int socketFd, newSocketFd; 								// File descriptor - To get state of sockets
	int clientLen,n;										// to stor the length of data being sent/received
	int count=0,i;
	struct arg arguments;
	pthread_t threads[MAXTHREADS];
	struct sockaddr_in serverAddr, clientAddr;				// struct to store server and client network config
	socketFd = socket(AF_INET,SOCK_STREAM,0);				// open socket
	if(socketFd<0)
		error("error opening socket");
	bzero((char *)&serverAddr,sizeof(serverAddr));			// Initialize all buffers to zero
	serverAddr.sin_family = AF_INET;						// Code for address family
	serverAddr.sin_port=htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;				// set server address to self
	if(bind(socketFd, (struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)		// bind socket to the address
		error("Error on binding");
	listen(socketFd,3);										// allow server to listen to 3 incoming connections at once
	while(count<threadCnt)
	{
	clientLen = sizeof(clientAddr);
	newSocketFd = accept(socketFd, (struct sockaddr*)&clientAddr,&clientLen);		// will cause system to block until it hears from a client
	if(newSocketFd<0)
		error("error on accepting data");
	arguments.blockSize=blockSize;
	arguments.newSocketFd=newSocketFd;
	pthread_create(&threads[count],NULL,&TCPServerThread,(void *)&arguments);
	count++;
	}
	for(i = 0; i < threadCnt; i++)
    {
      pthread_join(threads[i], NULL);
    }
	
}
void * TCPClientThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = NetworkTCPClient(arguments->blockSize,arguments->itrn,arguments->port,arguments->threadNo,arguments->host);
	printf("%f\tms\t\t%f\tMBps\n",LatencyInms(result),ThroughputInMBps(result,arguments->blockSize,arguments->itrn));

	pthread_exit(NULL);
}

void * UDPServerThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	char buffer[128000];
	while(1)
	{	
		bzero(buffer,arguments->blockSize);
		arguments->n = recvfrom(arguments->socketFd,buffer,arguments->blockSize,0,(struct sockaddr *)&arguments->clientAddr,&arguments->clientLen);		
		if(arguments->n<0)
			error("error reading from socket");
		arguments->n = sendto(arguments->socketFd,"message received UDP\n",25,0,(struct sockaddr *)&arguments->clientAddr,arguments->clientLen);
		if(arguments->n<0)
			error("error writing to socket");
	}
}


void NetworkUDPServer(long blockSize, long itrn, int port, int threadCnt)
{
	int socketFd, newSocketFd; 								// File descriptor - To get state of sockets
	int n,i,count=0;										// to stor the length of data being sent/received
	socklen_t clientLen;
	char buffer[128000];											// buffer to send or receive data
	pthread_t threads[MAXTHREADS];
	struct arg arguments;
	struct sockaddr_in serverAddr, clientAddr;				// struct to store server and client network config
	socketFd = socket(AF_INET,SOCK_DGRAM,0);				// open socket
	if(socketFd<0)
		error("error opening socket");
	bzero((char *)&serverAddr,sizeof(serverAddr));			// Initialize all buffers to zero
	serverAddr.sin_family = AF_INET;						// Code for address family
	serverAddr.sin_port=htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;				// set server address to self
	if(bind(socketFd, (struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)		// bind socket to the address
		error("Error on binding");
	listen(socketFd,3);						// allow server to listen to 3 incoming connections at once
	while(count<threadCnt)
	{
	clientLen = sizeof(clientAddr);
	arguments.blockSize=blockSize;
	arguments.newSocketFd=newSocketFd;
	arguments.socketFd=socketFd;
	arguments.clientLen = clientLen;
	arguments.clientAddr = clientAddr;
	pthread_create(&threads[count],NULL,&UDPServerThread,(void *)&arguments);
	count++;
	}
	for(i = 0; i < threadCnt; i++)
    {
      pthread_join(threads[i], NULL);
    }
}

void * UDPClientThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = NetworkUDPClient(arguments->blockSize,arguments->itrn,arguments->port,arguments->threadNo,arguments->host);
	printf("%f\tms\t\t%f\tMBps\n",LatencyInms(result),ThroughputInMBps(result,arguments->blockSize,arguments->itrn));

	pthread_exit(NULL);
}

int main(int argi, char *argc[]) {

	struct arg arguments;
	pthread_t threads[MAXTHREADS];

	int serverOrClient = atoi(argc[1]);
	int port = atoi(argc[2]);
	int protocol = atoi(argc[3]);
	int blockSize = atoi(argc[4]);
	int threadCnt = atoi(argc[5]);
	char *host = argc[6];
	int i;
	long itrn = 64000000/blockSize;
	itrn/=threadCnt;
	arguments.blockSize=blockSize;
	arguments.itrn=itrn;
	arguments.port=port;

	if(serverOrClient==1&&protocol==1)
			{
				arguments.threadNo=i;
				NetworkTCPServer(blockSize,itrn,port,threadCnt);
			}
	else if(serverOrClient==1&&protocol==2)
			{
				arguments.threadNo=i;
				NetworkUDPServer(blockSize,itrn,port,threadCnt);
			}

	else if(serverOrClient==2&&protocol==1)
		for(i=1;i<=threadCnt;i++)
			{
				arguments.threadNo=i;
				arguments.host = host;
				pthread_create(&threads[i],NULL,&TCPClientThread,(void *)&arguments);
				nanosleep((struct timespec[]){{0, 500000000}}, NULL);
			}
	else if(serverOrClient==2&&protocol==2)
		for(i=1;i<=threadCnt;i++)
			{
				arguments.threadNo=i;
				arguments.host=host;
				pthread_create(&threads[i],NULL,&UDPClientThread,(void *)&arguments);
			}
	pthread_exit(NULL);
}
