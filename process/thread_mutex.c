#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

int g_var = 1;
pthread_mutex_t mid;

void *inc_function(void *);
void *dec_function(void *);

int main(int argc, char** argv)
{
	pthread_t ptInc, ptDec;
	
	pthread_mutex_init(&mid, NULL);

	pthread_create(&ptInc,NULL, inc_function, NULL);
	pthread_create(&ptDec,NULL, dec_function, NULL);

	pthread_join(ptInc, NULL);
	pthread_join(ptDec, NULL);

	pthread_mutex_destroy(&mid);

	return 0;
}

void *inc_function(void *arg)
{
//	pthread_mutex_lock(&mid);
	int i=0;
//	for(i;i<5;++i){
printf("Inc : %d < Before\n", g_var);
	fflush(stdin);
	g_var++;

	printf("Inc : %d > After\n", g_var);

	fflush(stdin);
	//	pthread_mutex_unlock(&mid);
//	}
	return NULL;
}

void *dec_function(void *arg)
{

	int i=0;
//	for(i;i<5;++i){
//	pthread_mutex_lock(&mid);

	printf("Dec : %d < Before\n", g_var);
	fflush(stdin);
	g_var--;
	
	printf("Dec : %d > After\n", g_var);
	fflush(stdin);
//	pthread_mutex_unlock(&mid);
//	}
	return NULL;
}

