#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
	int n;
	int in;
	int out;

	char buf[1024];

	if(argc < 3){
		write(2, "Usage : copy file1 file2 \n", 25);
		
		return -1;
	}

	if((in=open(argv[1], O_RDONLY)) < 0)
	{
		perror(argv[1]);
		return -1;
	}

	if((out = open(argv[2], O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR)) < 0)
	{
		perror(argv[2]);
		return -1;
	}
//	close(out);

	printf("%d %d\n",in, out);
//	lseek(out, 0, SEEK_END);

	
	while((n=read(in, buf, sizeof(buf)))>0)
		write(out, buf,n);

	close(out);
	close(in);

	return 0;
}

