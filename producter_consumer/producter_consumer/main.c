#include <stdio.h>
#include <pthread.h>

#pragma comment(lib, "pthreadVC2.lib")

pthread_cond_t cond;
pthread_mutex_t mutex;

int count = 0, val = 0, loops = 100;

void put(int x)
{
	count = 1;
	val = x;
}

int get()
{
	count = 0;
	return val;
}

void *producer(void *arg)
{
	int i;
	for (i= 0 ; i < loops; i++)
	{
		pthread_mutex_lock(&mutex);
		if (count == 1)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		put(i);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *consumer(void *arg)
{
	int i;
	for (i = 0; i < loops; i++)
	{
		pthread_mutex_lock(&mutex);
		if (count == 0)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		int temp = get();
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		printf("%d\n", temp);
	}
	return NULL;
}

void main(void)
{
	int producer_id = 0, consumer_id = 0, i = 0;
	pthread_t producer_trd, consumer_trd;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	producer_id = pthread_create(&producer_trd, NULL, producer, NULL);
	consumer_id = pthread_create(&consumer_trd, NULL, consumer, NULL);

	getchar();

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}