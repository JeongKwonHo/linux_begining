/*	
 	@file	client.c
	@date	2018-06-03
	@author JeongKwonHo(jkh2802@gmail.com)
	@brief	chat program chat code
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024 
#define NAME_SIZE 20

void * send_msg(void * arg);
void * recv_msg(void * arg);

char name[NAME_SIZE];
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
	int ssock;
	int clen;
	struct sockaddr_in server_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	if(argc!=4) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		return -1;
	}

	sprintf(name, "%s", argv[3]);
	if((ssock=socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket()");
		return -1;
	}


	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	server_addr.sin_port=htons(atoi(argv[2]));

	clen = sizeof(server_addr);

	if(connect(ssock, (struct sockaddr*)&server_addr, clen)==-1)
	{
		perror("connect()");
		return -1;
	}

	printf("\n\n");
/*
	if(write(ssock, name, NAME_SIZE) <=0){
		perror("write()");
		return -1;
	}
*/
	printf("\n\n");
	printf("Welcom Chat Program!!\n\n");

	pthread_create(&snd_thread, NULL, send_msg, (void*)&ssock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&ssock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(ssock);  
	return 0;
}

void * send_msg(void * arg)
{
	int ssock=*((int*)arg);
	int i=0;
	char name_msg[NAME_SIZE+BUF_SIZE];

	while(1) 
	{
		fgets(msg, BUF_SIZE, stdin);
		sprintf(name_msg,"[%s] %s", name, msg);

		if(write(ssock, name_msg, BUF_SIZE)<=0){
			perror("write()");;
			return NULL;
		}

	}
	return NULL;
}

void * recv_msg(void * arg)   
{
	int ssock=*((int*)arg);
	char name_msg[BUF_SIZE];
	int str_len = 0;

	while(1)
	{
		str_len=read(ssock, name_msg, BUF_SIZE);

		fputs(name_msg, stdout);
	}
	return NULL;
}

