#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#define MAX_THREADS 512
#define MAX_WORDS 50

void *vow_fn( void * );
void *con_fn( void * );

char     *in_str[ MAX_WORDS ], c[ MAX_WORDS ], v[10] = { 'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
int       num_words = 0, t_threads = 0;
pthread_t tid1[ MAX_THREADS ], tid2[ MAX_THREADS ];

int main( int argc, char *argv[] )
{
    int            ret_val = 0, tmp, i = 1;
    pthread_attr_t attr;
	
	if(argv[1] == NULL)
	{
		return( ret_val );
	}
	
	while(argv[i] != NULL)
	{
		in_str[i-1]  = argv[i];
		c[i-1] = *in_str[i-1];
		i++;
	}
	num_words = i-1;
	for( i=0; i<num_words; i++)
	{
		pthread_attr_init ( &attr );
		pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );
			
		pthread_create( &tid1[i], &attr, vow_fn, (void *)i );
		pthread_create( &tid2[i], &attr, con_fn, (void *)i );
		
		pthread_join(tid1[i],NULL);
		pthread_join(tid2[i],NULL);
	}
	
	return( ret_val );
}

void *vow_fn( void *ii )
{
	int i, t = (int)ii;
	
	for( i=0; i<10; i++ )
	{
		if(c[t] == v[i])
		{
			printf("vow: %s\n", in_str[t]);
			pthread_exit(0);
		}
	}
	sched_yield();
    pthread_exit(0);
}

void *con_fn( void *ii )
{
	int i, t = (int)ii;
	
	for( i=0; i<10; i++ )
	{
		if(c[t] == v[i])
		{
			sched_yield();
			pthread_exit(0);
		}
	}
	
	printf("cons: %s\n", in_str[t]);
    pthread_exit(0);
}