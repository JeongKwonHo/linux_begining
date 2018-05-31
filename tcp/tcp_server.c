#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define TCP_PORT	5100

int main(int argc, char** argv)
{
	int ssock;
	socklen_t clen;
	int n;
	struct sockaddr_in client_addr, server_addr;
	char buf[BUFSIZ] = "Hello Mobis!";

	//Make Server Socket
	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket()");
		return -1;
	}

	//Set the socket address
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(TCP_PORT);

	//bind
	if(bind(ssock, (struct sockaddr*)&server_addr, sizeof(server_addr))<0) {
		perror("bind()");
		return -1;
	}

	//listen 
	if(listen(ssock, 8) <0 ){
		perror("listen()");
		return -1;
	}

	clen  = sizeof(client_addr);
	while(1)
	{
		//if the client connect the server, Make the client socket
		int csock = accept(ssock, (struct sockaddr*)&client_addr, &clen);

		printf("Client is connected : %s \n", inet_ntoa(client_addr.sin_addr));

		if((n = read(csock, buf, BUFSIZ)) <=0)
			perror("read()");
		
		printf("Received data : %s", buf);

		//send the message to server
		if(write(csock, buf, n) <=0)
			perror("write()");

		close(csock);
	}

	close(ssock);

	return 0;
}
	


