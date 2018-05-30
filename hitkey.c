#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int kbhit()
{
	struct termios oldt, newt; 			//터미널에 대한 구조체
	int ch, oldf;

	tcgetattr(0, &oldt);				//현재 터미널에 설정된 정보를 가져온다.
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);	//정규 모드 입력과 에코를 해제한다.
	tcsetattr(0, TCSANOW, &newt);		//새로운 값으로 터미널을 설정
	oldf = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, oldf | O_NONBLOCK);//입력을 넌블로킹 모드로 설정

	ch =  getchar();

	tcsetattr(0, TCSANOW, &oldt);		//기존의 값으로 터미널의 속성을 바로 적용
	fcntl(0, F_SETFL, oldf);

	if(ch !=EOF)
	{
		ungetc(ch, stdin);				//앞에서 읽은 위치 이전으로 포인터를 돌려준다.
		return 1;
	}

	return 0;

}

int main(int argc, char* argv[])
{
	int i=0;

	for(i=0;;i++)
	{
		if(kbhit()){
			switch(getchar()){
				case 'q':
					goto END;
					break;
			}
		}
		printf("%20d\t\t\r", i);
		usleep(100);
	}

END:
	printf("Good Bye!!\n");
	return 0;
}

