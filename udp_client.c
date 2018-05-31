#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define UDP_PORT    5100

int main(int argc, char** argv)
{
	int sockfd, n;
	socklen_t clisize;
	struct sockaddr_in servaddr, cliaddr;
	char sendline[BUFSIZ];
	char recvline[BUFSIZ];

	if(argc !=2){
		printf("usage : %s <IP address> \n", argv[0]);
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(UDP_PORT);

	while(fgets(sendline, BUFSIZ, stdin) != NULL){
		if(!strncmp(sendline, "q", 1)) break;
		sendto(sockfd, sendline, strlen(sendline), 0, 
				(struct sockaddr *)&servaddr, sizeof(servaddr));
		clisize = sizeof(cliaddr);

		n = recvfrom(sockfd, recvline, BUFSIZ, 0,
				(struct sockaddr*)&cliaddr, &clisize);
		recvline[n] = '\0';
		fputs(recvline, stdout);
	}
	close(sockfd);

	return 0;
}

