#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char** argv)
{
	int z, sock_fd[2];
	int status;
	char buf[] = "hello world", line[BUFSIZ];
	pid_t pid;

	z = socketpair(AF_LOCAL,SOCK_STREAM, 0, sock_fd);
	if(z == -1){
		perror("socketpair");
		return -1;
	}

	printf("socket 1 :%d\n", sock_fd[0]);
	printf("socket 2 :%d\n", sock_fd[1]);

	if((pid=fork())<0){
		perror("fork()");
	}
	else if(pid == 0){
		write(sock_fd[0], buf, strlen(buf)+1);
		printf("DATA send : %s\n", buf);

		close(sock_fd[0]);

	}
	else {
		wait(&status);
		
		read(sock_fd[1], line, BUFSIZ);
		printf("Received data : %s\n", line);

		close(sock_fd[1]);

	}


	return 0;
}
