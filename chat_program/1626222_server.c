/*
 	@file	server.c
	@date	2018-06-03
	@author	JeongKwonHo(jkh2802@gmail.com)
	@brief	chat program server code
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 128
#define MAX_CLNT 10
#define NAME_SIZE 20

void * handle_client(void * arg);

int client_cnt=0;
int client_socks[MAX_CLNT];

pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
	int ssock, csock;
	int i;

	struct sockaddr_in server_addr, client_addr;
	socklen_t clen;
	pthread_t t_id;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		return -1;
	}

	pthread_mutex_init(&mutx, NULL);
	
	//Make Server Socket
	if((ssock = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("socket()");
		return -1;
	}

	//Set the socket address
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET; 
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(atoi(argv[1]));

	//Bind
	if(bind(ssock, (struct sockaddr*) &server_addr, sizeof(server_addr))<0)
	{
		perror("bind()");
		return -1;	
	}
	
	//Listen
	if(listen(ssock, 10) < 0 )
	{
		perror("listen()");
		return -1;
	}

	clen = sizeof(client_addr);

	while(1)
	{
		//If the client connect the server, Make the client socket
		csock = accept(ssock, (struct sockaddr*)&client_addr, &clen);

		if(client_cnt >= MAX_CLNT) {
			printf("CONNECT FAIL : %d \n", csock);
			write(csock, "too many users. sorry", BUF_SIZE);
			continue;
		}


		pthread_mutex_lock(&mutx);

		client_socks[client_cnt++]=csock;
		pthread_mutex_unlock(&mutx);


		pthread_create(&t_id, NULL, handle_client, (void*)&csock);
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(client_addr.sin_addr));
	}
	close(ssock);
	return 0;
}

void * handle_client(void * arg)
{
	int csock=*((int*)arg);
	int str_len=0;
	int i;

	char msg[BUF_SIZE] ;

	while((str_len=read(csock, msg, BUF_SIZE))!=0) 
	{

		int i;
		pthread_mutex_lock(&mutx);
		for(i=0; i<client_cnt; i++)
		{	
			if(csock !=client_socks[i])
				write(client_socks[i], msg, BUF_SIZE);
		}
		pthread_mutex_unlock(&mutx);
	}


	pthread_mutex_lock(&mutx);
	for(i=0; i<client_cnt; i++)  
	{
		if(csock==client_socks[i])
		{
			while(i++<client_cnt-1) {
				client_socks[i]=client_socks[i+1];
			}
			break;
		}
	}
	client_cnt--;
	pthread_mutex_unlock(&mutx);
	close(csock);
	return (void*)NULL;
}
